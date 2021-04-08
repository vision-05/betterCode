package bettercode.codearea;

import java.util.AbstractList;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
import java.util.List;

import javafx.beans.InvalidationListener;
import javafx.beans.property.BooleanProperty;
import javafx.beans.property.DoubleProperty;
import javafx.beans.property.IntegerProperty;
import javafx.beans.property.SimpleDoubleProperty;
import javafx.beans.value.ChangeListener;
import javafx.beans.value.WritableValue;
import javafx.collections.ListChangeListener;
import javafx.css.CssMetaData;
import javafx.css.StyleConverter;
import javafx.css.StyleableBooleanProperty;
import javafx.css.StyleableIntegerProperty;
import javafx.css.StyleableProperty;

import com.sun.javafx.binding.ExpressionHelper;
import com.sun.javafx.collections.ListListenerHelper;
import com.sun.javafx.collections.NonIterableChange;
import javafx.css.converter.SizeConverter;
import javafx.scene.control.skin.TextAreaSkin;

import javafx.css.Styleable;
import javafx.scene.AccessibleRole;

import javafx.scene.control.TextInputControl;
import javafx.collections.ObservableList;

public class BetterCodeArea extends TextInputControl {
    private static final class CodeAreaContent implements Content {
        private ExpressionHelper<String> helper = null;
        private ArrayList<StringBuilder> paragraphs = new ArrayList<StringBuilder>();
        private int contentLength = 0;
        private ParagraphList paragraphList = new ParagraphList();
        private ListListenerHelper<CharSequence> listenerHelper;

        private CodeAreaContent() {
            paragraphs.add(new StringBuilder(32));
            paragraphList.content = this;
        }

        @Override public String get(int start, int end) {
            int length = end - start;
            StringBuilder text = new StringBuilder(length);

            int startLineIndex = 0;
            int endLineIndex = 0;
            int characterCount = 0;

            int index = start;

            for(int i = 0; i < paragraphs.size(); i++) {
                if(characterCount + paragraphs.get(i).length() <= index) {
                    startLineIndex++;
                    endLineIndex++;
                    startCharacterLineOffset += paragraphs.get(i).length();
                    characterCount += paragraphs.get(i).length();
                }
                else if(endLineIndex == 0) {
                    start -= characterCount;
                    if(characterCount + paragraphs.get(i).length() >= start && characterCount + paragraphs.get(i).length() <= end) {
                        endLineIndex++;
                        characterCount += paragraphs.get(i).length();
                    }
                }
                else if(characterCount + paragraphs.get(i).length() >= start && characterCount + paragraphs.get(i).length() <= end) {
                    endLineIndex++;
                    characterCount += paragraphs.get(i).length();
                }
            }

            for(int i = 0; i < length; i++) {
                for(int j = start; j < paragraphs.get(startLineIndex).length(); j++) {
                    char c = paragraphs.get(startLineIndex).charAt(j);
                    text.append(c);
                }
                text.append('\n');
                start = 0;
            }

            return text.toString();
        }
        //done
        @Override public void insert(int index, String text, boolean notifyListeners) {
            if(index < 0 || index > contentLength) {
                throw new IndexOutOfBoundsException();
            }

            if(text == null) {
                throw new IllegalArgumentException();
            }

            text = TextInputControl.filterInput(text, false, false);
            int length = text.length();

            int characterCount = 0;
            int startLineIndex = 0;

            for(int i = 0; i < paragraphs.size(); i++) {
                if(characterCount + paragraphs.get(i).length() <= index) {
                    startLineIndex++;
                    characterCount += paragraphs.get(i).length();
                }
            }
            index -= characterCount;

            ArrayList<StringBuilder> textLines = new ArrayList<StringBuilder>();
            StringBuilder line = new StringBuilder(length); //change this length value
            for(int i = 0; i < text.length(); i++) {
                if(text.charAt(i) == '\n') {
                    textLines.add(line);
                    line = new StringBuilder(length);
                }
                else {
                    line.append(textLines.charAt(i));
                }
            }

            int endLineIndex = textLines.length() + startLineIndex - 1;

            ArrayList<StringBuilder> tempParagraphs = new ArrayList<StringBuilder>();
            Collections.copy(tempParagraphs, paragraphs);
            //change to case for single line insert...
            //insert textLines into paragraphs, use copy and swap
            if(textLines.length() == 1) {
                tempParagraphs.get(startLineIndex).insert(index, text);
                fireParagraphListChangeEvent(index, index + 1, Collections.singletonList((CharSequence)tempParagraphs.get(startLineIndex)));
            }
            else {
                tempParagraphs.get(startLineIndex).append(textLines.get(0));
                fireParagraphListChangeEvent(index, index + 1, Collections.singletonList((CharSequence)tempParagraphs.get(startLineIndex)));
                tempParagraphs.addAll(startLineIndex + 1, textLines.subList(1, textLines.size()));
                fireParagraphListChangeEvent(startLineIndex + 1, startLineIndex + textLines.size(), Collections.singletonList.EMPTY_LIST);
            
                line = tempParagraphs.get(endLineIndex);
                textLines.get(textLines.length() - 1).append(line);
                tempParagraphs.set(endLineIndex, textLines.get(textLines.length() - 1));
                fireParagraphListChangeEvent(endLineIndex, endLineIndex + 1, Collections.singletonList((CharSequence)tempParagraphs.get(endLineIndex)));
            }

            Collections.copy(paragraphs, tempParagraphs);
            contentLength += length;

            if(notifyListeners) {
                ExpressionHelper.fireValuesChangedEvent(helper);
            }
        }
        //add listener stuff
        @Override public void delete(int start, int end, boolean notifyListeners) {
            if(start < 0 || start > contentLength || end > contentLength || end < 0) {
                throw new IndexOutOfBoundsException();
            }

            int startLineIndex = 0;
            int endLineIndex = 0;

            int characterCount = 0;
            int startCharacterLineOffset = 0;

            if(end < start) {
                int temp = end;
                end = start;
                start = temp;
            }

            int deleteLength = end - start;

            for(int i = 0; i < paragraphs.size(); i++) {
                if(characterCount + paragraphs.get(i).length() <= start) {
                    startLineIndex++;
                    endLineIndex++;
                    startCharacterLineOffset += paragraphs.get(i).length();
                    characterCount += paragraphs.get(i).length();
                }
                else if(characterCount + paragraphs.get(i).length() >= start && characterCount + paragraphs.get(i).length() <= end) {
                    endLineIndex++;
                    characterCount += paragraphs.get(i).length();
                }
            }

            start -= startCharacterLineOffset;
            end -= characterCount;

            ArrayList<StringBuilder> tempParagraphs = new ArrayList<StringBuilder>();
            Collections.copy(tempParagraphs, paragraphs);

            int removeLineDiff = (endLineIndex - startLineIndex) - 1;

            if(removeLineDiff != 0) {
                for(int i = 0; i < removeLineDiff; i++) {
                    tempParagraphs.remove(startLineIndex + 1); //other rows shift left 1
                }
                endLineIndex -= removeLineDiff;

                tempParagraphs.get(startLineIndex).delete(start, tempParagraphs.get(startLineIndex).size() - 1);
                tempParagraphs.get(endLineIndex).delete(0, end);
            }
            else {
                tempParagraphs.get(startLineIndex).delete(start, end);
            }

            Collections.copy(paragraphs, tempParagraphs); //copy and swap
            contentLength -= deleteLength;

            if(notifyListeners) {
                ExpressionHelper.fireValuesChangedEvent(helper);
            }
        }

        @Override public int length() {
            return this.contentLength;
        }
    }

    private static final class ParagraphList extends AbstractList<CharSequence> implements ObservableList<CharSequence> {
        
    }

    private static final class ParagraphListChange extends NonIterableChange<CharSequence> {

    }

    public BetterCodeArea(String text) {

    }

    public BetterCodeArea() {
        this("");
    }
}
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
            //implement rest here
            return text.toString();
        }

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
            start -= characterCount;

            ArrayList<StringBuilder> textLines = new ArrayList<StringBuilder>();
            StringBuilder line = new StringBuilder(length);
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

            //insert textLines into paragraphs, use copy and swap
            tempParagraphs.get(startLineIndex).append(textLines.get(0));
            for(int i = startLineIndex + 1; i < endLineIndex - 1; i++) {
                tempParagraphs.add(i, textLines.get(i - (startLineIndex + 1))
            }
            
            line = tempParagraphs.get(endLineIndex);
            //change below to .set() method
            tempParagraphs.remove(endLineIndex);
            tempParagraphs.add(textLines.get(textLines.length() - 1));
            tempParagraphs.get(endLineIndex).append(line);
        }

        @Override public void delete(int start, int end, boolean notifyListeners) {
            if(start < 0 || start > contentLength || end > contentLength || end < 0) {
                throw new IndexOutOfBoundsException();
            }

            int startLineIndex = 0;
            int endLineIndex = 0;

            int characterCount = 0;

            if(end < start) {
                int temp = end;
                end = start;
                start = temp;
            }

            for(int i = 0; i < paragraphs.size(); i++) {
                if(characterCount + paragraphs.get(i).length() <= start) {
                    startLineIndex++;
                    endLineIndex++;
                    characterCount += paragraphs.get(i).length();
                }
                else if(characterCount + paragraphs.get(i).length() >= start && characterCount + paragraphs.get(i).length() <= end) {
                    endLineIndex++;
                    characterCount += paragraphs.get(i).length();
                }
            }

            //start -= characterCount (get offset into line);
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

            paragraphs = tempParagraphs; //maybe copy
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
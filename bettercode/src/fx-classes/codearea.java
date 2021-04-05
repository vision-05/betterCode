package javafx.scene.control;

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

public class CodeArea extends TextInputControl {
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
                throw new IllegalArgumetnException();
            }

            text = TextInputControl.filterInput(text, false, false);
            int length = text.length();

            //implement rest here
        }

        @Override public void delete(int start, int end, boolean notifyListeners) {
            if(index < 0 || index > contentLength || end > contentLength || end < 0) {
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
                if(characterCount + paragraphs[i].size() <= start) {
                    startLineIndex++;
                    endLineIndex++;
                    characterCount += paragraphs[i].size();
                }
                start -= characterCount;
                else if(characterCount + paragraphs[i].size() >= start && characterCount + paragraphs[i].size() <= end) {
                    endLineIndex++;
                    characterCount += paragraphs[i].size();
                }
                end -= characterCount;
            }

            ArrayList<StringBuilder> tempParagraphs = new ArrayList<StringBuilder>(paragraphs);

            int removeLineDiff = (endLineIndex - startLineIndex) - 1;

            if(removeLineDiff) {
                for(int i = 0; i < removeLineDiff; i++) {
                    tempParagraphs.remove(startLineIndex + 1); //other rows shift left 1
                }
                endLineIndex -= removeLineDiff;

                tempParagraphs[startLineIndex].delete(start, tempParagraphs[startLineIndex].size() - 1);
                tempParagraphs[endLineIndex].delete(0, end);
            }
            else {
                tempParagraphs[startLineIndex].delete(start, end);
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

    public CodeArea(String) {

    }

    public CodeArea() {
        this("");
    }
}
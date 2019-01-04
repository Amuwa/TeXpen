// Copyright (C) 2013 Alex-97
// This file is part of textpad-editor.
//
// textpad-editor is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// textpad-editor is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with textpad-editor.  If not, see <http://www.gnu.org/licenses/>.

#include "headers/mainwindow.h"
#include <QAction>


QString commentColor="#aaffaa";
void MainWindow::CreateStyle(int SelectedTheme) {
    Settings.ColorScheme = SelectedTheme;

    QString Color;
    QString BackgroundColor;
    QString SelectionColor;
    QString SelectionBackgroundColor;
    QString StyleSheet;

    switch (Settings.ColorScheme) {
    case 1:
        // This is the "Console" theme
        Color = "rgb(200, 200, 200)";
        BackgroundColor = "#000000";
        SelectionColor = "#000000";
        SelectionBackgroundColor = "rgb(200, 200, 200)";
        commentColor="#ddcc88";
        ConsoleColor->setChecked(true);
        break;
    case 2:
        // This is the "Dusk" theme
        Color = "";
        BackgroundColor = "";
        SelectionColor = "";
        SelectionBackgroundColor = "";
        DuskColor->setChecked(true);
        commentColor="#2aff3a";
        break;
    case 3:
        // This is the "Hacker" theme
        Color = "#00CC00";
        BackgroundColor = "#000000";
        SelectionColor = "#000000";
        SelectionBackgroundColor = "#00CC00";
        commentColor="#ff22ed";
        HackerColor->setChecked(true);
        break;
    case 4:
        // This is the "Light" theme
        Color = "#000000";
        BackgroundColor = "rgb(240, 240, 240)";
        SelectionColor = "rgb(200, 200, 200)";
        SelectionBackgroundColor = "rgb(40, 40, 40)";
        LightColor->setChecked(true);
        commentColor="#11ff0a";
        break;
    case 5:
        // This is the "Navy" theme
        Color = "rgb(237, 247, 241)";
        BackgroundColor = "rgb(16, 28, 42)";
        SelectionColor = "rgb(237, 247, 241)";
        SelectionBackgroundColor = "rgb(24, 97, 129)";
        commentColor="#4aff4a";
        NavyColor->setChecked(true);
        break;
    case 6:
        // This is the "Night" theme
        Color = "rgb(234, 234, 234)";
        BackgroundColor = "rgb(63, 63, 63)";
        SelectionColor = "rgb(63, 63, 63)";
        SelectionBackgroundColor = "rgb(206, 255, 132)";
        commentColor="#8aff8f";
        NightColor->setChecked(true);
        break;
    case 7:
        // This is the "Normal" theme
        Color.clear();
        BackgroundColor.clear();
        SelectionColor.clear();
        SelectionBackgroundColor.clear();
        commentColor="#66ff66";
        NormalColor->setChecked(true);
        break;
    case 8:
        // This is the "Paper" theme
        Color = "#000000";
        BackgroundColor = "rgb(248, 248, 248)";
        SelectionColor = "#000000";
        SelectionBackgroundColor = "rgb(206, 255, 132)";
        commentColor="#66ff66";
        PaperColor->setChecked(true);
        break;
    }

    QString StyleSheet2;
    if (SelectedTheme != 7) {
        // Create the stylesheet with the selected values
        StyleSheet = ("QPlainTextEdit {"
                      "color: " + Color + ";"
                      "background-color: " + BackgroundColor + ";"
                      "selection-color: " + SelectionColor + ";"
                      "selection-background-color: " + SelectionBackgroundColor + ";"
                      "};"
                      );
        StyleSheet2 = (
                    "QTreeWidget {"
                 "color: " + Color + ";"
                 "background-color: " + BackgroundColor + ";"
                 "selection-color: " + SelectionColor + ";"
                 "selection-background-color: " + SelectionBackgroundColor + ";"
                    "};"
                 );
    }

    else if (SelectedTheme == 7) {
        // Use the system style (Normal Color)
        StyleSheet.clear();
        StyleSheet2.clear();
    }

    TextEdit->setStyleSheet(StyleSheet);
    structure->setStyleSheet(StyleSheet2);
    if(highlighter!=NULL){
        highlighter->rehighlight();
    }
}

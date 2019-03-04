TeXpen - Your LaTeX Editor
==========================

[Qt4.7 or Higher Version is recommended]

ABOUT:
	TeXpen is a text editor, especially for LaTeX users.

WEBPAGE:
	http://www.journalhome.com/texpen/

ACKNOWLEDGEMENT:
	Based on another project, textpad-editor (http://textpad-editor.sf.net)
	Thanks to OSCHINA.NET for the git server

FAST BUILD:
	qmake 
	make

BUILD:
	For Linux/Unix users:
		Install Qt development packages (qt-sdk or qt4-* )
		Using QtCreator to build the project "texpen.pro",  OR 
		qmake & make

	For OS X users:
        Install Xcode from Mac App Store.
        Install Qt develop package from Homebrew:
        brew install qt
        brew cask install qt-creator
        then using QtCreator to build the project "texpen.pro"

	For Windows users:
		Install Qt developing packages (qt-sdk)
		Using QtCreator to build the project "texpen.pro"

Features include:
- Document Outline/Structure
- Equation Preview (realtime)
- PDF preview
- Themes(background/foreground colors)
- Highlighting
- Print Source Codes
- Auto-Completion 
- Spell-Checking 
- Grammar and expression checking and suggestion
- Figure/Table snippets
- Build/Configure Building Commands 
- Document Templates
- Complex LaTeX project management (in planning)
- Biography management (in planning)


Changelog:

周一 3月 9 01:20:12 2015
- Add line number
- Improve I/O efficiency

周五 7月 25 14:35:57 2014
- Add command flow support

周三 7月 16 12:54:04 2014
- Add PDF preview (F7)
- Add Ctrl+M looking up word in online dictionary function 
- Add Ctrl+/ grammar and expression check and suggestion function.
- Add Ctrl+= Wiki looking up
- Add Ctrl+,  "embeded" google search
- Add Academic Word List auto-completion function.
- Fix the Enter key capture issue 



周日 7月 6 00:37:10 2014
- Update the algorithm of buiding the structure/outline
- Add Article and Slide templates


周二 7月 23 11:51:32 2013
- Fix the last-line-keyword scanning problem


周一 7月 22 16:08:05 2013
- Add Spell-checking feature (en_GB/en_US checking is provided)
- Improve Find/Replace feature


周五 7月 19 12:51:18 2013
- Add Find/Replace Feature (by Alex S. http://textpad-editor.sf.net )
- Improve tips of build/stop building
- Fix svg icon problem on Windows

周三 7月 17 22:44:43 2013
- Add auto-completion feature

周三 7月 17 15:10:51 2013
- Add Command Configuration Dialog
- Add Building Termination feature
- Fix Building Log Scroll problem
- other little fixes

周一 7月 15 14:19:51 2013
- Add Table and Figure/Graphic Insertion Feature
- Fix Outline Theme Problem
- Some little fixes

周六 7月 13 12:39:32 2013
- Fix datatime insertion problem
- Add recent files list
- Fix no-save-quit problem
- Fix outline auto-hide problem




Group Members:
2030026036 Auberon
2030026117 Joshua
2030026116 Vicky

Before executing the program, you need to do following things:
1. Set up Qt environment
   1.1 Set up MySQL database environment
        Here, we use 32-bit MariaDB Connector C to connect the database.
        1.11 Compile Qt MySQL driving
	reset mysql.pro file in the install path of your own Qt.
	(a) comment "QMAKE_USE += mysql"
	(b) Open the program folder -> MariaDB Connector C -> 
               	      Copy the path of include folder
                      Copy the path of lib folder
	(c) Paste lib folder path after "LIBS += -L'<libpath>' -llibmariadb"
	     Example: win32: LIBS += -L'C:/Program Files (x86)/MariaDB/MariaDB Connector C/lib/' -llibmariadb
	(d) Paste include folder path after "INCLUDEPATH += '<include path>'"
	     Example: INCLUDEPATH += 'C:/Program Files (x86)/MariaDB/MariaDB Connector C/include'
	(e) Paste lib folder path after "DEPENDPATH += '<lib folder path>'"
	     Example: DEPENDPATH += 'C:/Program Files (x86)/MariaDB/MariaDB Connector C/lib'
	(f) Add the deatination directory
	    After last step, add one line of code:
	    DESTDIR = ../mysql/lib/
	(g) Use your desktop to compile and run this .pro file
	      If success, a new folder called 'lib' will appear under mysql.pro current directory and go to 1.12. 
        1.12 Add mysql dynamic link libraries
	(a) In last step lib path, copy two dll files under to mysql path
	Path Example :'D:/Qt/Qt5.12.10/5.12.10/Src/qtbase/src/plugins/sqldrivers/mysql/lib/
	.dll files name: qsqlmysql.dll and qsqlmysqld.dll
         1.13  Add mysql include files and libraries to  Qt environment
	Copy the files and folders in MariaDB Connector C and paste them under your Qt source mysql folder:
	'/Qt/Qt5.12.10/5.12.10/Src/qtbase/src/plugins/sqldrivers/mysql/'
   1.2 Set up server environment
        1.21 Add mysql library dependencies and winsocket libraries
	Change all path to your own Qt:
	LIBS += -L'D:/Qt/Qt5.12.10/5.12.10/Src/qtbase/src/plugins/sqldrivers/mysql/lib' -llibmariadb
	LIBS += -lwsock32
	INCLUDEPATH += 'D:/Qt/Qt5.12.10/5.12.10/Src/qtbase/src/plugins/sqldrivers/mysql/include'
	DEPENDPATH += 'D:/Qt/Qt5.12.10/5.12.10/Src/qtbase/src/plugins/sqldrivers/mysql/include'
        1.22 Compile this server.pro in 32-bit Kits:
	Here we recommend to use: 
	Desktop Qt 5.12.10 MinGW 32-bit
   1.3 Set up client environment
        1.31 open Qt Creator -> Tool -> Performance -> Device -> Android ->
	set up Java Settings and Android Settings (including Android SDK and Android NDK)
	click 'Apply'
2. Run projects
   2.1 Open server.pro in Qt creator
         Right click server.pro and click compile
         Right click again and run the server
         Remind: Compile this server.pro in 32-bit Kits, here we recommend to use: Desktop Qt 5.12.10 MinGW 32-bit
   2.2 Open client in Qt creator
         Open Project1_1128.pro in Qt Creator, this is client.
         Right click Project1_1128.pro and click compile.
         Deploy to Android Device:
         Here, use kit that fit your test mobile devices.
         We tested our project on Android Qt 5.12.10 Clang arm64-v8a
         Right click again and run the client.
   2.3 Open USB debug Mode on your mobile phone when deploy this project.
         Remenber to use USB line to connect your mobile phone to your Desktop.

Remind: before input course_info.sql to your database, you should create a database name as 'course_info' first.
For DataBase, we set the ip address as 'localhost' , username as 'root' and the password is empty in our codes.

Then you can run the program successfully!
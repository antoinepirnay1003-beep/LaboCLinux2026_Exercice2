CC = g++
CFLAGS = -pipe -g -std=gnu++11 -Wall -W -D_REENTRANT -fPIC -DQT_DEPRECATED_WARNINGS -DQT_QML_DEBUG -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I../UNIX_Exercice2 -I. -isystem /usr/include/qt5 -isystem /usr/include/qt5/QtWidgets -isystem /usr/include/qt5/QtGui -isystem /usr/include/qt5/QtCore -I. -I. -I/usr/lib64/qt5/mkspecs/linux-g++
LFLAGS = /usr/lib64/libQt5Widgets.so /usr/lib64/libQt5Gui.so /usr/lib64/libQt5Core.so /usr/lib64/libGL.so -lpthread

OBJS = main.o mywindow.o moc_mywindow.o FichierUtilisateur.o
EXEC = CLINUX_Exercice2

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) -o $(EXEC) $(OBJS) $(LFLAGS)

main.o: main.cpp
	$(CC) -c $(CFLAGS) -o main.o main.cpp

mywindow.o: mywindow.cpp
	$(CC) -c $(CFLAGS) -o mywindow.o mywindow.cpp

moc_mywindow.o: moc_mywindow.cpp
	$(CC) -c $(CFLAGS) -o moc_mywindow.o moc_mywindow.cpp

FichierUtilisateur.o: FichierUtilisateur.cpp
	$(CC) -c -o FichierUtilisateur.o FichierUtilisateur.cpp

clean:
	rm -f *.o $(EXEC)

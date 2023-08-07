===============================================================================
                          Data Cleaner für Morrowind
                                von Thoronador
                                (30. Mai 2011)
===============================================================================


Zweck des Programmes
====================

Das Programm Data Cleaner soll dazu dienen, ungenutzte Dateien (gegenwärtig nur
Meshes und Icons) im Data Files-Verzeichnis von Morrowind zu finden und zu lö-
schen. Dazu werden alle vorhandenen Plugins nach benötigten Dateien durchsucht
und alle anderen Dateien im entsprechenden Unterverzeichnis gelöscht.

Das Programm funktioniert mit jeder Sprachversion von Morrowind.


Programmversionen
=================

Wenn nichts anderes angegeben ist, so beziehen sich die Angaben in dieser Datei
allesamt auf die Version 0.2 des Programmes, welche am 08. Dezember 2021 er-
stellt wurde. In neueren Programmversionen kann sich die Funktionalität des
Programmes ändern oder neue Funktionen hinzukommen, die hier noch nicht doku-
mentiert sind.


Programmaufruf
==============

Der Aufruf erfolgt über die Kommandozeile, das Programm selbst kann in ein be-
liebiges Verzeichnis entpackt werden und muss nicht im Morrowindverzeichnis
landen. Ein typischer Aufruf sieht so aus:

  data_cleaner.exe -d "D:\Spiele\Bethesda\Morrowind\Data Files"

Zu der Pfadangabe: hinter -d muss der Pfad zum Data Files-Verzeichnis angegeben
werden, in dem sich die Master-Dateien von Morrowind befinden. Diese Angabe
kann man auch weglassen, allerdings wird dann ein Standardverzeichnis (z.Z. ist
das C:\Program Files\Bethesda Softworks\Morrowind\Data Files) benutzt, sodass
das nicht auf jedem System funktioniert oder das gewünschte Ergebnis produziert.


Parameter
=========

In diesem Abschnitt folgt eine Liste der möglichen Programmparameter für
Data Cleaner. Alle Parameter sind optional, das heißt, sie müssen nicht ange-
geben werden. Allerdings kann das Fehlen eines Parameters dazu führen, dass das
Programm nicht korrekt ausgeführt werden kann und abbricht. Deshalb sollte man
in der Regel mindestens den Pfad zum Data Files-Verzeichnis von Morrowind ange-
ben, indem man den Parameter -d benutzt.


--data-files VERZEICHNIS  - legt das Verzeichnis VERZEICHNIS als Data Files-
                            Verzeichnis von Morrowind fest. In diesem Verzeich-
                            nis müssen Morrowind.esm und alle andere Plugin-
                            dateien, welche nach benötigten Dateien durchsucht
                            werden sollen, zu finden sein. Existiert das Ver-
                            zeichnis nicht, wird das Programm vorzeitig ab-
                            brechen.
-d VERZEICHNIS            - Kurzform von --data-files
--help                    - zeigt einen Hilfetext zu den Parametern des Pro-
                            grammes an und beendet das Programm. Alle anderen
                            Parameter werden dann verworfen und es werden keine
                            Dateien gelöscht.
-?                        - Kurzform von --help
--version                 - zeigt die Version des Programmes an und beendet das
                            Programm. Alle anderen Parameter werden dann ver-
                            worfen und es werden keine Dateien gelöscht.
--all                     - lädt alle Master- und Plugindateien aus dem angege-
                            benen Data Files-Verzeichnis und durchsucht diese
                            nach benötigten Dateien (Standardeinstellung).
--all-data-files          - identisch zu --all
--explicit                - startet den expliziten Modus, in dem alle zu durch-
                            suchenden Master- und Plugindateien explizit angege-
                            ben werden müssen (siehe dazu die Parameter -f und
                            --ini). Diese Option und --all schließen sich gegen-
                            seitig aus, es kann nur eine davon angegeben werden.
-e                        - Kurzform von --explicit
--add-file Plugin.esp     - fügt die Datei Plugin.esp zur Liste der zu durchsu-
                            chenden Plugins hinzu. Dateien aus Plugin.esp werden
                            dann nicht gelöscht. Dieser Parameter kann wieder-
                            holt werden, um so mehrere Plugindateien anzugeben.
                            Dieser Parameter ist nur im expliziten Modus er-
                            laubt.
-f Plugin.esp             - Kurzform von --add-file
--ini                     - liest die Plugindateien aus dem Abschnitt
                            [Game Files] der Morrowind.ini aus und fügt diese
                            Dateien der Liste der zu durchsuchenden Plugins
                            hinzu. Ist keine Morrowind.ini vorhanden (was bei
                            einer normalen Morrowindinstallation nicht passie-
                            ren sollte), so bricht das Programm ab und erzeugt
                            keine Plugindatei. Dieser Parameter ist nur im ex-
                            pliziten Modus erlaubt.
-i                        - Kurzform von --ini


Lizenz und Quellcode
====================

Das Programm data_cleaner.exe steht unter der GNU General Public Licence 3,
einer freien Softwarelizenz. Der volle Text der Lizenz ist in der Datei GPL.txt
enthalten und kann auch auf http://www.gnu.org/licenses/gpl-3.0.html eingesehen
werden.

Das Programm wurde in der Hoffnung, dass es nützlich ist, erstellt und verfügbar
gemacht. Das Programm ist nicht vollendet und kann daher Fehler („bugs“) ent-
halten. Aus den genannten Gründen wird es unter dieser Lizenz „so wie es ist“
ohne jegliche Gewährleistung zur Verfügung gestellt. Dies gilt unter anderem
– aber nicht ausschließlich – für Verwendbarkeit für einen bestimmten Zweck,
Mängelfreiheit und Richtigkeit (siehe dazu die entsprechenden Abschnitte der
GNU General Public Licence 3).

Der Quellcode des Programms lässt sich auf Sourceforge.net einsehen, das
Projekt findet sich unter http://sourceforge.net/projects/morrowtools/


Danksagung
==========

Vielen Dank an Bethesda Softworks für The Elder Scrolls III: Morrowind.
Ohne dieses tolle Spiel würde es dieses Programm auch nicht geben.
Mein besonderer Dank gilt Dave Humphrey von UESP.net für seine Übersicht zum
ESM-Dateiformat, welche mir beim Erstellen des Programms eine große Hilfe war.
Jene Übersicht findet sich auf http://www.uesp.net/morrow/tech/mw_esm.txt
wieder.

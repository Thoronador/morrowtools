===============================================================================
                          Spell Renamer f�r Morrowind
                                von Thoronador
                               (30. April 2011)
===============================================================================


Zweck des Programmes
====================

Das Programm Spell Renamer soll dazu dienen, die Zauber im Zaubermen� von
Morrowind nach Zugeh�rigkeit zu den Zauberschulen zu sortieren, indem ein ent-
sprechendes Plugin erstellt wird, welches die Zaubernamen �ndert. Dazu wird je-
dem Zauber der Name seiner Schule vorangestellt - also B bei Beschw�rung, I bei
Illusion, M bei Mystik, V bei Ver�nderung, W bei Wiederherstellung, Z bei Zer-
st�rung.

Dadurch entstehen dann Zaubernamen wie beispielsweise:

B Beschworenes Langschwert
I Unsichtbarkeit
M Almsivi Intervention
V Festes Wasser
Z Feuerball

Dies funktioniert auch mit der englischen Version von Morrowind. Dort werden den
Zaubern dann aber entsprechend andere Buchstaben vorangestellt (A bei Alter-
ation, C bei Conjuration, D bei Destruction, I bei Illusion, M bei Mysticism,
R bei Restoration). Prinzipiell sollte das Programm mit jeder Sprachversion von
Morrowind zurechtkommen und die vorangestellten Buchstaben entsprechend der
Sprache der Master- und Plugindateien anpassen, da hierf�r die zugeh�rigen GMST-
Werte ausgelesen werden, aber bisher wurde es nur mit der deutschen und der eng-
lischen Version getestet.


Weitere Buchstabenmuster
========================

In zuk�nftigen Versionen des Programmes wird es m�glicherweise mehrere Buchsta-
benmuster oder Kombinationen aus Buchstaben und Ziffern geben, welche dem Namen
eines Zaubers je nach Wahl vorangestellt werden k�nnen. Allerdings sind mir da-
f�r bisher keine sinnvollen Muster eingefallen. Wer eine Idee dazu hat, kann
sich ja bei mir melden und mir diese mitteilen, vielleicht wird diese dann auch
in das Programm eingebaut. Jedoch sollten diese Kombinationen nicht l�nger als
drei oder vier Zeichen sein, da es sonst Probleme bei den l�ngeren Zaubernamen
geben k�nnte.


Programmversionen
=================

Wenn nichts anderes angegeben ist, so beziehen sich die Angaben in dieser Datei
allesamt auf die Version 0.1_rev219 des Programmes, welche am 30.April 2011 er-
stellt wurde. In neueren Programmversionen kann sich die Funktionalit�t des
Programmes �ndern oder neue Funktionen hinzukommen, die hier noch nicht doku-
mentiert sind.


Programmaufruf
==============

Der Aufruf erfolgt �ber die Kommandozeile, das Programm selbst kann in ein be-
liebiges Verzeichnis entpackt werden und muss nicht im Morrowindverzeichnis
landen. Ein typischer Aufruf sieht so aus:

  spell_rename.exe -d "D:\Spiele\Bethesda\Morrowind\Data Files" -o "Neue Datei.esp"

Zu den zwei Pfadangaben: hinter -d muss der Pfad zum Data Files-Verzeichnis an-
gegeben werden, in dem sich die Master-Dateien von Morrowind befinden. Nach -o
kann man den Namen der neu zu erstellenden .esp-Datei angeben. Diese wird dann
im gleichen Verzeichnis erstellt, man sollte also Schreibrechte in diesem Ver-
zeichnis haben. Beide Angaben kann man auch weglassen, allerdings wird dann ein
Standardverzeichnis (z.Z. C:\Program Files\Bethesda Softworks\Morrowind\
Data Files) und ein vorgegebener Dateiname (out.esp) benutzt, sodass das nicht
auf jedem System funktioniert oder das gew�nschte Ergebnis produziert.

Falls man noch weitere Plugins, z.B. Zauberplugins, mit einbeziehen m�chte,
l�sst sich das nun auch bewerkstelligen, indem man dies zus�tzlich nach dem
Parameter -f angibt:

  spell_rename.exe -d "D:\Spiele\Bethesda\Morrowind\Data Files" -o "Neue Datei.esp" -f "Zauberplugin.esp"

Dies kann man auch beliebig oft f�r weitere Plugins wiederholen.
Folgendes w�re also auch denkbar:

  spell_rename.exe -d "D:\Spiele\Bethesda\Morrowind\Data Files" -o "Neue Datei.esp" -f "Zauberplugin.esp" -f "anderes Plugin.esp" -f "VieleTolleZauber.esp"

Voraussetzung ist nat�rlich, dass die angegebenen Plugins auch existieren.
Werden die Plugins nicht gefunden, bricht das Programm ab.


Parameter
=========

In diesem Abschnitt folgt eine Liste der m�glichen Programmparameter f�r
Spell Renamer. Alle Parameter sind optional, das hei�t, sie m�ssen nicht ange-
geben werden. Allerdings kann das Fehlen eines Parameters dazu f�hren, dass das
Programm nicht korrekt ausgef�hrt werden kann und vor dem Erstellen des Plugins
abbricht. Deshalb sollte man in der Regel mindestens den Pfad zum Data Files-
Verzeichnis von Morrowind angeben, indem man den Parameter -d benutzt.


--data-files VERZEICHNIS  - legt das Verzeichnis VERZEICHNIS als Data Files-
                            Verzeichnis von Morrowind fest. In diesem Verzeich-
                            nis m�ssen Morrowind.esm und alle andere Plugin-
                            dateien, welche verarbeitet werden sollen, zu finden
                            sein. Existiert das Verzeichnis nicht, wird das
                            Programm vorzeitig abbrechen und keine Plugindatei
                            erstellen.
-d VERZEICHNIS            - Kurzform von --data-files
--output NeuesPlugin.esp  - legt NeuesPlugin.esp als Name f�r die neu erstellte
                            Plugindatei fest. Wird dieser Parameter nicht ange-
                            geben, so hei�t die Ausgabedatei einfach nur
                            out.esp.
-o NeuesPlugin.esp        - Kurzform von --output
--add-files Plugin.esp    - f�gt die Datei Plugin.esp zur Liste der zu durchsu-
                            chenden Plugins hinzu. Zauber aus Plugin.esp werden
                            dann ebenfalls mit dem Buchstabenmuster versehen.
                            Dieser Parameter kann wiederholt werden, um so
                            mehrere Plugindateien anzugeben.
-f Plugin.esp             - Kurzform von --add-files
--ini                     - liest die Plugindateien aus dem Abschnitt
                            [Game Files] der Morrowind.ini aus und f�gt diese
                            Dateien der Liste der zu durchsuchenden Plugins
                            hinzu. Ist keine Morrowind.ini vorhanden (was bei
                            einer normalen Morrowindinstallation nicht passie-
                            ren sollte), so bricht das Programm ab und erzeugt
                            keine Plugindatei.
-i                        - Kurzform von --ini
--ini-with-discard        - wie --ini, allerdings werden zuvor alle m�glicher-
                            weise �ber den Parameter --add-files bzw. -f hin-
                            zugef�gten Plugindateien aus der Pluginliste
                            herausgenommen.
--allow-truncate          - Normalerweise werden diejenigen Zaubernamen vom
                            Programm nicht ge�ndert, welche durch das Hinzu-
                            f�gen des Buchstabenmusters der Zauberschule zu
                            lang werden, um Fehler beim Laden des Plugins im
                            Spiel zu vermeiden. Aus diesem Grund kann es aber
                            dazu kommen, dass einige Zauber nicht entsprechend
                            sortiert werden. Durch Angabe dieses Parameters
                            werden solche Zauber auch mit einem Buchstabenmuster
                            versehen, allerdings kann es dann dazu kommen, dass
                            einige Buchstaben am Ende des Zaubernamens abge-
                            schnitten sind, oder dass es beim Laden des erstell-
                            ten Plugins zu Fehlermeldungen kommt.
--verbose                 - zeigt ein paar Informationen zu den verarbeiteten
                            Plugindateien an.
--silent                  - Gegenteil von --verbose; zeigt jene Informationen
                            nicht an.
--help                    - zeigt einen Hilfetext zu den Parametern des Pro-
                            grammes an und beendet das Programm. Alle anderen
                            Parameter werden dann verworfen und es wird keine
                            Plugindatei erstellt.
-?                        - Kurzform von --help
--version                 - zeigt die Version des Programmes an und beendet das
                            Programm. Alle anderen Parameter werden dann ver-
                            worfen und es wird keine Plugindatei erstellt.


Lizenz und Quellcode
====================

Das Programm spell_renamer.exe steht unter der GNU General Public Licence 3,
einer freien Softwarelizenz. Der volle Text der Lizenz ist in der Datei GPL.txt
enthalten und kann auch auf http://www.gnu.org/licenses/gpl-3.0.html eingesehen
werden.

Der Quellcode des Programms l�sst sich auf Sourceforge.net einsehen, das
Projekt findet sich unter http://sourceforge.net/projects/morrowtools/


Danksagung
==========

Vielen Dank an Bethesda Softworks f�r The Elder Scrolls III: Morrowind.
Ohne dieses tolle Spiel w�rde es dieses Programm auch nicht geben.
Mein besonderer Dank gilt Dave Humphrey von UESP.net f�r seine �bersicht zum
ESM-Dateiformat, welche mir beim Erstellen des Programms eine gro�e Hilfe war.
Jene �bersicht findet sich auf http://www.uesp.net/morrow/tech/mw_esm.txt
wieder.
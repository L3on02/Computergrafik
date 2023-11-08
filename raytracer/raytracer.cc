#include "geometry.h"
#include "camera.h"
#include "fileout.h"
#include "object.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <thread>

// Die folgenden Kommentare beschreiben Datenstrukturen und Funktionen
// Die Datenstrukturen und Funktionen die weiter hinten im Text beschrieben sind,
// hängen höchstens von den vorhergehenden Datenstrukturen ab, aber nicht umgekehrt.



// Ein "Bildschirm", der das Setzen eines Pixels kapselt
// Der Bildschirm hat eine Auflösung (Breite x Höhe)
// Kann zur Ausgabe einer PPM-Datei verwendet werden oder
// mit SDL2 implementiert werden.



// Eine "Kamera", die von einem Augenpunkt aus in eine Richtung senkrecht auf ein Rechteck (das Bild) zeigt.
// Für das Rechteck muss die Auflösung oder alternativ die Pixelbreite und -höhe bekannt sein.
// Für ein Pixel mit Bildkoordinate kann ein Sehstrahl erzeugt werden.



// Für die "Farbe" benötigt man nicht unbedingt eine eigene Datenstruktur.
// Sie kann als vec3 implementiert werden mit Farbanteil von 0 bis 1.
// Vor Setzen eines Pixels auf eine bestimmte Farbe (z.B. 8-Bit-Farbtiefe),
// kann der Farbanteil mit 255 multipliziert  und der Nachkommaanteil verworfen werden.


// Das "Material" der Objektoberfläche mit ambienten, diffusem und reflektiven Farbanteil.



// Ein "Objekt", z.B. eine Kugel oder ein Dreieck, und dem zugehörigen Material der Oberfläche.
// Im Prinzip ein Wrapper-Objekt, das mindestens Material und geometrisches Objekt zusammenfasst.
// Kugel und Dreieck finden Sie in geometry.h/tcc


// verschiedene Materialdefinition, z.B. Mattes Schwarz, Mattes Rot, Reflektierendes Weiss, ...
// im wesentlichen Variablen, die mit Konstruktoraufrufen initialisiert werden.


// Die folgenden Werte zur konkreten Objekten, Lichtquellen und Funktionen, wie Lambertian-Shading
// oder die Suche nach einem Sehstrahl für das dem Augenpunkt am nächsten liegenden Objekte,
// können auch zusammen in eine Datenstruktur für die gesammte zu
// rendernde "Szene" zusammengefasst werden.

// Die Cornelbox aufgebaut aus den Objekten
// Am besten verwendet man hier einen std::vector< ... > von Objekten.

// Punktförmige "Lichtquellen" können einfach als vec3 implementiert werden mit weisser Farbe,
// bei farbigen Lichtquellen müssen die entsprechenden Daten in Objekt zusammengefaßt werden
// Bei mehreren Lichtquellen können diese in einen std::vector gespeichert werden.

// Sie benötigen eine Implementierung von Lambertian-Shading, z.B. als Funktion
// Benötigte Werte können als Parameter übergeben werden, oder wenn diese Funktion eine Objektmethode eines
// Szene-Objekts ist, dann kann auf die Werte teilweise direkt zugegriffen werden.
// Bei mehreren Lichtquellen muss der resultierende diffuse Farbanteil durch die Anzahl Lichtquellen geteilt werden.

// Für einen Sehstrahl aus allen Objekte, dasjenige finden, das dem Augenpunkt am nächsten liegt.
// Am besten einen Zeiger auf das Objekt zurückgeben. Wenn dieser nullptr ist, dann gibt es kein sichtbares Objekt.

// Die rekursive raytracing-Methode. Am besten ab einer bestimmten Rekursionstiefe (z.B. als Parameter übergeben) abbrechen.

void populate_world(std::vector<Object>& world) {
  color red = {0.8f, 0.3f, 0.3f};
  color green = {0.3f, 0.8f, 0.3f};
  color white = {0.8f, 0.8f, 0.8f};

  Object sphere1 = {sphere3({0.0f, 0.0f, -5.0f}, 0.5f), red};
  world.push_back(sphere1);
  Object sphere2 = {sphere3({0.0f, 0.0f, -5.0f}, 1.0f), green};
  world.push_back(sphere2);
}

int main(void) {
  point3 cam_center = {0.0f, 0.0f, -10.0f};
  float focal_length = 1.0f;
  float vfov = 90.0f;
  int image_width = 600;
  float aspect_ratio = 16.0f/9.0f;
  int image_height = image_width / aspect_ratio;

  camera cam(cam_center, focal_length, vfov, image_width, image_height, aspect_ratio);

  std::vector<Object> world;
  populate_world(world);

  fileout file(image_width, image_height);

  for(int i = 0; i < image_height; i++) {
    for(int j = 0; j < image_width; j++) {
      ray3 ray = cam.get_ray(i, j);
      color pixel_color = {0.0f, 0.0f, 0.0f};
      
      for (auto& obj : world) {
        if (obj.sphere.intersects(ray) > 0.0f) {
          pixel_color = obj.col;
        }
      }
      file.writeColor(pixel_color);
    }
  }


  return 0;   
}
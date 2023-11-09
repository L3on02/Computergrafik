#include "geometry.h"
#include "camera.h"
#include "fileout.h"
#include "hitable.h"

#include <vector>
#include <algorithm>
#include <limits>
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

void populate_world(std::vector<hitable> &world)
{
  color red = {0.8f, 0.3f, 0.3f};
  color green = {0.3f, 0.8f, 0.3f};
  color white = {0.8f, 0.8f, 0.8f};

  world.push_back({{{0, -10000, 0}, 9990}, white});
  world.push_back({{{0, 10000, 0}, 9990}, white});
  world.push_back({{{0, 0,-10000}, 8000}, white});
  world.push_back({{{-10000, 0, 0}, 9990}, red});
  world.push_back({{{10000, 0, 0}, 9990}, green});
  
}

int main(void)
{
  point3 cam_center = {0.0f, 0.0f, 0.0f};
  float focal_length = 1.0f;
  float vfov = 30.0f;
  int image_width = 600;
  float aspect_ratio = 1.0f;
  int image_height = image_width / aspect_ratio;

  camera cam(cam_center, focal_length, vfov, image_width, image_height, aspect_ratio);

  std::vector<hitable> world;
  populate_world(world);

  fileout file(image_width, image_height);

  for (int i = 0; i < image_height; i++)
  {
    for (int j = 0; j < image_width; j++)
    {
      ray3 ray = cam.get_ray(i, j);
      color pixel_color = {0.0f, 0.0f, 0.0f};

      // render:
      hitable closest;
      float closest_t = std::numeric_limits<float>::max();
      for (auto obj : world)
      {
        float t = obj.sphere.intersects(ray);
        if (t > 0.0f)
        {
          if (t < closest_t)
          {
            closest = obj;
            closest_t = t;
          }
        }
      }
      pixel_color = closest.col;

      file.writeColor(pixel_color);
    }
  }

  return 0;
}
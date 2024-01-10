#ifndef WAVEFRONT_H
#define WAVEFRONT_H

#include <fstream>
#include <vector>
#include <array>
#include <string>
#include <map>

typedef std::array<float, 3> Vertice;
typedef std::array<float, 3> Normal;
typedef std::array<float, 3> Color;

// only that ambient color will be stored
struct Material {
  Color ambient;
};

struct ReferenceGroup {
  Vertice vertice;
  Normal normal;
};

// all points (reference_groups[i].vertice) should be in a common plane,
// the form a polygon if the are all oriented clock- or counter-clock-wise
struct Face {
  std::vector<ReferenceGroup> reference_groups;
  Material * material; // optional material information
};

class WavefrontImporter {
  bool counter_clock_wise;
  size_t input_line;
  std::istream & in;
  Material * current_material;

  std::vector< Vertice > vertices;
  std::vector< Normal > normals;
  std::vector< Face > faces;
  std::map<std::string, Material> materials;

  void warn(std::string error);
  void err(std::string error);
  float parse_float(std::istream & );
  std::vector<float> parse_floats(std::istream & );
  void parse_vertex_data();
  void parse_face();
  void parse_use_material();
  void parse_material_library();
public:
  WavefrontImporter(std::istream & in);
  
  // parses the input stream as a char-stream forming a  wavefront file
  // stores the vertices, normals, and faces
  // texture coordinates are ignored
  void parse();
  
  // parses the input stream as a char-stream froming a wavefront material file
  // the materials are stored by their name into a map 
  // only ambient colors (r,g,b in the value range [0.0, 1.0]) are stored for a material, other information is ignored
  // alpha-values of colors are ignored
  void parse_material(std::istream & in);

  std::vector< Vertice > & get_vertices();
  std::vector< Normal > & get_normals();
  std::vector< Face > & get_faces();

  void set_materials( std::map<std::string, Material> materials);
  
  // returns a map containing all materials
  // a material is stored under its name (as given in the material file) 
  std::map<std::string, Material> & get_materials();

  bool faces_order_is_counter_clock_wise() const;
};

#endif
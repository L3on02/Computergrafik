#include "wavefront.h"

#include <string>
#include <iostream>
#include <limits>

WavefrontImporter::WavefrontImporter(std::istream & in) 
  : counter_clock_wise(true), input_line(0u), in(in), current_material(nullptr) { }


void WavefrontImporter::warn(std::string warning) {
  std::cerr << input_line << ":warning: " << warning << std::endl;
}

void WavefrontImporter::err(std::string error) {
  std::cerr << input_line << ":error:  " << error << std::endl;
}

std::vector< Vertice > & WavefrontImporter::get_vertices() {
  return vertices;
}
std::vector< Normal > & WavefrontImporter::get_normals() {
  return normals;
}

std::vector< Face > & WavefrontImporter::get_faces() {
  return faces;
}

std::map<std::string, Material> & WavefrontImporter::get_materials() {
  return materials;
}

void WavefrontImporter::set_materials( std::map<std::string, Material> materials) {
  this->materials = materials;
}


float WavefrontImporter::parse_float(std::istream & in) {
  float f = -1.0f;
  
  in >> f;
  if (in.fail()) {
    err("fail to read in a float");
  }
  return f;
}

// reads in three floats until a end of line
std::vector<float> WavefrontImporter::parse_floats(std::istream & in) {
  std::vector<float> floats;
  
  floats.push_back( parse_float(in) );
  floats.push_back( parse_float(in) );
  floats.push_back( parse_float(in) );
  
  return floats;
}

void WavefrontImporter::parse_vertex_data() {
  std::vector<float> floats;
  char c = in.peek();
  
  switch (c) {
  case 't': warn("texture vertices found and ignored (not supported)");
            in >> c;
            break;    //vt Texture vertices (IGNORED)
  case 'n': in >> c;
            floats = parse_floats(in);  //vn Vertex normals
            normals.push_back( {floats[0], floats[1], floats[2]} );
            break;
  case 'p': warn("parameter space vertices found and ignored (not supported)");
            in >> c;
            break;    //vp Parameter space vertices (IGNORED)
  default:  floats = parse_floats(in); //v Geometric vertices
            vertices.push_back({floats[0], floats[1], floats[2]});
  }

}



// no texture coordinates supported
void WavefrontImporter::parse_face() {
  // f v1//v1n v2//v2n v3//v3n 
  // f v1 v2 v3
  Face face;
  size_t v1, v2, v3,
         vn1 = 0, vn2, vn3;

  in >> v1;
  if (in.peek() == '/') in.ignore(); // no texture coordinates supported
  if (in.peek() == '/') { in.ignore(); in >> vn1;}
  in >> v2;
  if (in.peek() == '/') in.ignore(); 
  if (in.peek() == '/') { in.ignore(); in >> vn2;}
  in >> v3;
  if (in.peek() == '/') in.ignore(); 
  if (in.peek() == '/') { in.ignore(); in >> vn3;}
 
  if (vn1 == 0) {
    warn("no normals given");
    // calculate normal not done
    Normal normal = {1.0f, 1.0f, 1.0f};
    face.reference_groups.push_back( { vertices.at(v1 - 1), normal } );
    face.reference_groups.push_back( { vertices.at(v2 - 1), normal } );
    face.reference_groups.push_back( { vertices.at(v3 - 1), normal } );
  } else {
    face.reference_groups.push_back( { vertices.at(v1 - 1), normals.at(vn1 - 1) } );
    face.reference_groups.push_back( { vertices.at(v2 - 1), normals.at(vn2 - 1) } );
    face.reference_groups.push_back( { vertices.at(v3 - 1), normals.at(vn3 - 1) } );
  }
  if (current_material != nullptr) {
    face.material = current_material;
  } else {
    warn("no material set for face");
  }
  faces.push_back( face );
}

void WavefrontImporter::parse_use_material() {
  std::string s;
  in >> s;
  if (s == "semtl") {
    in >> s;
    erase_if(s, [](char c) { return isspace(c); });
    if (materials.find(s) != materials.end() ) {
      Material * new_material = &materials.at(s);
      current_material = new_material;
    }
  } else {
    warn("usemtl expected");
  }
}

void WavefrontImporter::parse_material_library() {
  std::string s;
  in >> s;
  if (s == "tllib") {
    in >> s;
    std::fstream fs(s);  
    parse_material(fs); 
  } else {
    warn("mtllib expected");
  }
}


void WavefrontImporter::parse() {
  std::vector<float> floats;
  char c;
  while (in >> c) {
    switch (c) {
    case 'v': parse_vertex_data();
              break;
    case 'f': parse_face();
              break;
    case 'u': parse_use_material();
              break;
    case 'm': parse_material_library();
              break;
    }
    // read to end of line; skips comments, too
    input_line++;
    in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }

}

void WavefrontImporter::parse_material(std::istream & in) {
  std::string s;
  std::string material_name;
  while (in >> s)  {
    if ( s == "newmtl" ) {
      in >> material_name;      
    } else if ( s == "Kd" ) {
      std::vector<float> floats = parse_floats(in);
      if (s.size() < 3) {
        Color color = {floats[0], floats[1], floats[2]};
        
        materials[material_name] = { color };
      } else {
        err("not enough float values for a RGB-color");
      }
    }
    in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }
}



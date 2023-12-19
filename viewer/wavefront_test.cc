#include "wavefront.h"
#include <iostream>
#include <sstream>
#include <fstream>

#include "gtest/gtest.h"

namespace {
	
TEST(WAVEFRONT_IMPORTER, Init1) {
  WavefrontImporter importer(std::cin);
}

TEST(WAVEFRONT_IMPORTER, Init2) {
  std::stringstream ss("");
  WavefrontImporter importer(ss);
}

TEST(WAVEFRONT_IMPORTER, ParseVertice) {
  std::stringstream ss("v 1.0 2.0 -0.4");
  WavefrontImporter importer(ss);
  
  importer.parse();

  auto vertices = importer.get_vertices();

  ASSERT_EQ(1, vertices.size());  
  ASSERT_EQ(0, importer.get_normals().size());  
  auto vertice = vertices[0];    
  ASSERT_NEAR(1.0f, vertice[0], 0.00001f);
  ASSERT_NEAR(2.0f, vertice[1], 0.00001f);
  ASSERT_NEAR(-0.4f, vertice[2], 0.00001f);
}

TEST(WAVEFRONT_IMPORTER, ParseVertice2) {
  std::stringstream ss("# Blender v2.78 (sub 0) OBJ File: \n"
                       "# www.blender.org\n"
                       "o teapot_large.001\n"
                       "v 1.368074 2.435437 -0.227403\n"
                       "v 1.381968 2.400000 -0.229712\n"
                       "v 1.400000 2.400000 0.000000\n");
  WavefrontImporter importer(ss);
  importer.parse();

  auto vertices = importer.get_vertices();

  ASSERT_EQ(3, vertices.size());  
  ASSERT_EQ(0, importer.get_normals().size());  
}




TEST(WAVEFRONT_IMPORTER, ParseVertices) {
  std::stringstream ss("v 1.0 2.0 -0.4\nv 0.0 -1.0 0.0\n");
  WavefrontImporter importer(ss);
  
  importer.parse();

  auto vertices = importer.get_vertices();

  ASSERT_EQ(2, vertices.size());  
  ASSERT_EQ(0, importer.get_normals().size());  
  auto vertice = vertices[0];    
  ASSERT_NEAR(1.0f, vertice[0], 0.00001f);
  ASSERT_NEAR(2.0f, vertice[1], 0.00001f);
  ASSERT_NEAR(-0.4f, vertice[2], 0.00001f);
  vertice = vertices[1];    
  ASSERT_NEAR(0.0f, vertice[0], 0.00001f);
  ASSERT_NEAR(-1.0f, vertice[1], 0.00001f);
  ASSERT_NEAR(0.0f, vertice[2], 0.00001f);
}

TEST(WAVEFRONT_IMPORTER, ParseNormals) {
  std::stringstream ss("vn 1.0 2.0 -0.4\n"
                       "vn 1.0 1.0 0.0\n"
                       "vn 0.0 -1.0 0.0\n");
  WavefrontImporter importer(ss);
  
  importer.parse();

  auto vertices = importer.get_vertices();
  auto normals = importer.get_normals();
  
  ASSERT_EQ(0, vertices.size());  
  ASSERT_EQ(3, normals.size());
  auto normal = normals[0];    
  ASSERT_NEAR(1.0f, normal[0], 0.00001f);
  ASSERT_NEAR(2.0f, normal[1], 0.00001f);
  ASSERT_NEAR(-0.4f, normal[2], 0.00001f);
  normal = normals[1];    
  ASSERT_NEAR(1.0f, normal[0], 0.00001f);
  ASSERT_NEAR(1.0f, normal[1], 0.00001f);
  ASSERT_NEAR(0.0f, normal[2], 0.00001f);
  normal = normals[2];    
  ASSERT_NEAR(0.0f, normal[0], 0.00001f);
  ASSERT_NEAR(-1.0f, normal[1], 0.00001f);
  ASSERT_NEAR(0.0f, normal[2], 0.00001f);
}

TEST(WAVEFRONT_IMPORTER, ParseFacesVertices) {
std::stringstream ss( "v 1.000000 -1.000000 -1.000000\n"
                      "v 1.000000 -1.000000 1.000000\n"
                      "v -1.000000 -1.000000 1.000000\n"
                      "v -1.000000 -1.000000 -1.000000\n"
                      "v 1.000000 1.000000 -1.000000\n"
                      "v 1.000000 1.000000 1.000001\n"
                      "v -1.000000 1.000000 1.000000\n"
                      "v -1.000000 1.000000 -1.000000\n"
                      "vn 0.000000 -1.000000 0.000000\n"
                      "vn 0.000000 1.000000 0.000000\n"
                      "vn 1.000000 -0.000000 0.000000\n"
                      "vn -0.000000 -0.000000 1.000000\n"
                      "vn -1.000000 -0.000000 -0.000000\n"
                      "vn 0.000000 0.000000 -1.000000\n"
                      "f 2//1 3//1 4//1\n"
                      "f 8//2 7//2 6//2\n"
                      "f 1//3 5//3 6//3\n"
                      "f 2//4 6//4 7//4\n"
                      "f 7//5 8//5 4//5\n"
                      "f 1//6 4//6 8//6\n"
                      "f 1//1 2//1 4//1\n"
                      "f 5//2 8//2 6//2\n"
                      "f 2//3 1//3 6//3\n"
                      "f 3//4 2//4 7//4\n"
                      "f 3//5 7//5 4//5\n"
                      "f 5//6 1//6 8//6\n");
  WavefrontImporter importer(ss);
  importer.parse();

  auto vertices = importer.get_vertices();
  auto normals = importer.get_normals();
  auto faces = importer.get_faces();
  ASSERT_EQ(8, vertices.size());  
  ASSERT_EQ(6, normals.size());
  ASSERT_EQ(12, faces.size());
  Face face = faces.at(0);
  ReferenceGroup group = face.reference_groups[0];
  ASSERT_NEAR(1.0f, group.vertice[0], 0.00001f);
  ASSERT_NEAR(-1.0f, group.vertice[1], 0.00001f);
  ASSERT_NEAR(1.0f, group.vertice[2], 0.00001f);
  face = faces.at(5);
  group = face.reference_groups[1];
  ASSERT_NEAR(-1.0f, group.vertice[0], 0.00001f);
  ASSERT_NEAR(-1.0f, group.vertice[1], 0.00001f);
  ASSERT_NEAR(-1.0f, group.vertice[2], 0.00001f);
}

TEST(WAVEFRONT_IMPORTER, ParseFacesNormals) {
std::stringstream ss( "v 1.000000 -1.000000 -1.000000\n"
                      "v 1.000000 -1.000000 1.000000\n"
                      "v -1.000000 -1.000000 1.000000\n"
                      "v -1.000000 -1.000000 -1.000000\n"
                      "v 1.000000 1.000000 -1.000000\n"
                      "v 1.000000 1.000000 1.000001\n"
                      "v -1.000000 1.000000 1.000000\n"
                      "v -1.000000 1.000000 -1.000000\n"
                      "vn 0.000000 -1.000000 0.000000\n"
                      "vn 0.000000 1.000000 0.000000\n"
                      "vn 1.000000 -0.000000 0.000000\n"
                      "vn -0.000000 -0.000000 1.000000\n"
                      "vn -1.000000 -0.000000 -0.000000\n"
                      "vn 0.000000 0.000000 -1.000000\n"
                      "f 2//1 3//1 4//1\n"
                      "f 8//2 7//2 6//2\n"
                      "f 1//3 5//3 6//3\n"
                      "f 2//4 6//4 7//4\n"
                      "f 7//5 8//5 4//5\n"
                      "f 1//6 4//6 8//6\n"
                      "f 1//1 2//1 4//1\n"
                      "f 5//2 8//2 6//2\n"
                      "f 2//3 1//3 6//3\n"
                      "f 3//4 2//4 7//4\n"
                      "f 3//5 7//5 4//5\n"
                      "f 5//6 1//6 8//6\n");
  WavefrontImporter importer(ss);
  importer.parse();

  auto vertices = importer.get_vertices();
  auto normals = importer.get_normals();
  auto faces = importer.get_faces();
  ASSERT_EQ(8, vertices.size());  
  ASSERT_EQ(6, normals.size());
  ASSERT_EQ(12, faces.size());
  Face face = faces.at(0);
  ReferenceGroup group = face.reference_groups[0];
  ASSERT_NEAR(0.0f, group.normal[0], 0.00001f);
  ASSERT_NEAR(-1.0f, group.normal[1], 0.00001f);
  ASSERT_NEAR(0.0f, group.normal[2], 0.00001f);
  face = faces.at(1);
  group = face.reference_groups[0];
  ASSERT_NEAR(0.0f, group.normal[0], 0.00001f);
  ASSERT_NEAR(1.0f, group.normal[1], 0.00001f);
  ASSERT_NEAR(0.0f, group.normal[2], 0.00001f);
  face = faces.at(2);
  group = face.reference_groups[1];
  ASSERT_NEAR(1.0f, group.normal[0], 0.00001f);
  ASSERT_NEAR(0.0f, group.normal[1], 0.00001f);
  ASSERT_NEAR(0.0f, group.normal[2], 0.00001f);
  face = faces.at(3);
  group = face.reference_groups[1];
  ASSERT_NEAR(0.0f, group.normal[0], 0.00001f);
  ASSERT_NEAR(0.0f, group.normal[1], 0.00001f);
  ASSERT_NEAR(1.0f, group.normal[2], 0.00001f);
  face = faces.at(4);
  group = face.reference_groups[1];
  ASSERT_NEAR(-1.0f, group.normal[0], 0.00001f);
  ASSERT_NEAR(0.0f, group.normal[1], 0.00001f);
  ASSERT_NEAR(0.0f, group.normal[2], 0.00001f);
  face = faces.at(5);
  group = face.reference_groups[2];
  ASSERT_NEAR(0.0f, group.normal[0], 0.00001f);
  ASSERT_NEAR(0.0f, group.normal[1], 0.00001f);
  ASSERT_NEAR(-1.0f, group.normal[2], 0.00001f);
  face = faces.at(6);
  group = face.reference_groups[1];
  ASSERT_NEAR(0.0f, group.normal[0], 0.00001f);
  ASSERT_NEAR(-1.0f, group.normal[1], 0.00001f);
  ASSERT_NEAR(0.0f, group.normal[2], 0.00001f);
  face = faces.at(7);
  group = face.reference_groups[1];
  ASSERT_NEAR(0.0f, group.normal[0], 0.00001f);
  ASSERT_NEAR(1.0f, group.normal[1], 0.00001f);
  ASSERT_NEAR(0.0f, group.normal[2], 0.00001f);
  face = faces.at(11);
  group = face.reference_groups[1];
  ASSERT_NEAR(0.0f, group.normal[0], 0.00001f);
  ASSERT_NEAR(0.0f, group.normal[1], 0.00001f);
  ASSERT_NEAR(-1.0f, group.normal[2], 0.00001f);
}

TEST(WAVEFRONT_IMPORTER, ParseFacesMaterial) {
std::stringstream ss( "v 1.000000 -1.000000 -1.000000\n"
                      "v 1.000000 -1.000000 1.000000\n"
                      "v -1.000000 -1.000000 1.000000\n"
                      "v -1.000000 -1.000000 -1.000000\n"
                      "v 1.000000 1.000000 -1.000000\n"
                      "v 1.000000 1.000000 1.000001\n"
                      "v -1.000000 1.000000 1.000000\n"
                      "v -1.000000 1.000000 -1.000000\n"
                      "vn 0.000000 -1.000000 0.000000\n"
                      "vn 0.000000 1.000000 0.000000\n"
                      "vn 1.000000 -0.000000 0.000000\n"
                      "vn -0.000000 -0.000000 1.000000\n"
                      "vn -1.000000 -0.000000 -0.000000\n"
                      "vn 0.000000 0.000000 -1.000000\n"
                      "usemtl red\n"
                      "f 2//1 3//1 4//1\n"
                      "f 8//2 7//2 6//2\n"
                      "usemtl blue\n"
                      "f 1//3 5//3 6//3\n"
                      "f 2//4 6//4 7//4\n"
                      "usemtl green\n"
                      "f 7//5 8//5 4//5\n"
                      "f 1//6 4//6 8//6\n"
                      "usemtl yellow\n"
                      "f 1//1 2//1 4//1\n"
                      "f 5//2 8//2 6//2\n"
                      "usemtl cyan\n"
                      "f 2//3 1//3 6//3\n"
                      "f 3//4 2//4 7//4\n"
                      "usemtl magenta\n"
                      "f 3//5 7//5 4//5\n"
                      "f 5//6 1//6 8//6\n");
  std::map<std::string, Material> materials;
  materials["red"] = { {1.0f, 0.0f, 0.0f} };
  materials["blue"] = { {0.0f, 0.0f, 1.0f} };
  materials["green"] = { {0.0f, 1.0f, 0.0f} };
  materials["yellow"] = { {0.0f, 1.0f, 0.0f} };
  materials["cyan"] = { {1.0f, 1.0f, 0.0f} };
  materials["magenta"] = { {1.0f, 0.0f, 1.0f} };
  WavefrontImporter importer(ss);
  importer.set_materials(materials);
  importer.parse();

  auto faces = importer.get_faces();
  
  Face face = faces[11];
  Color color = face.material->ambient;
  ASSERT_NEAR(1.0f, color[0], 0.00001f);
  ASSERT_NEAR(0.0f, color[1], 0.00001f);
  ASSERT_NEAR(1.0f, color[2], 0.00001f);
}

TEST(WAVEFRONT_IMPORTER, ParseMaterial) {
std::stringstream ss( "newmtl red\n"
                      "Kd 1.000 0.000 0.000\n"
                      "newmtl green\n"
                      "Kd 0.000 1.000 0.000\n"
                      "newmtl blue\n"
                      "Kd 0.000 0.000 1.000\n"
                      "newmtl magenta\n"
                      "Kd 1.000 0.000 1.000\n"
                      "newmtl cyan\n"
                      "Kd 0.000 1.000 1.000\n"
                      "newmtl yellow\n"
                      "Kd 1.000 1.000 0.000\n" );  
  WavefrontImporter importer(ss);               
  importer.parse_material(ss);                                       
  std::map<std::string, Material> materials = importer.get_materials();
  
  ASSERT_EQ(6, materials.size());
  Color color = materials["cyan"].ambient;
  ASSERT_NEAR(0.0f, color[0], 0.00001f);
  ASSERT_NEAR(1.0f, color[1], 0.00001f);
  ASSERT_NEAR(1.0f, color[2], 0.00001f);
}

TEST(WAVEFRONT_IMPORTER, ParseMaterialFromFile) {
  std::fstream fs("../objects/basic.mtl");  
  WavefrontImporter importer(fs);               
  importer.parse_material(fs);                                       
  std::map<std::string, Material> 
  materials = importer.get_materials();
  
  ASSERT_EQ(6, materials.size());
  Color color = materials["red"].ambient;
  ASSERT_NEAR(1.0f, color[0], 0.00001f);
  ASSERT_NEAR(0.0f, color[1], 0.00001f);
  ASSERT_NEAR(0.0f, color[2], 0.00001f);
}

TEST(WAVEFRONT_IMPORTER, ParseAll) {
std::stringstream ss( "mtllib ../objects/basic.mtl\n"
                      "v 1.000000 -1.000000 -1.000000\n"
                      "v 1.000000 -1.000000 1.000000\n"
                      "v -1.000000 -1.000000 1.000000\n"
                      "v -1.000000 -1.000000 -1.000000\n"
                      "v 1.000000 1.000000 -1.000000\n"
                      "v 1.000000 1.000000 1.000001\n"
                      "v -1.000000 1.000000 1.000000\n"
                      "v -1.000000 1.000000 -1.000000\n"
                      "vn 0.000000 -1.000000 0.000000\n"
                      "vn 0.000000 1.000000 0.000000\n"
                      "vn 1.000000 -0.000000 0.000000\n"
                      "vn -0.000000 -0.000000 1.000000\n"
                      "vn -1.000000 -0.000000 -0.000000\n"
                      "vn 0.000000 0.000000 -1.000000\n"
                      "usemtl red\n"
                      "f 2//1 3//1 4//1\n"
                      "f 8//2 7//2 6//2\n"
                      "usemtl blue\n"
                      "f 1//3 5//3 6//3\n"
                      "f 2//4 6//4 7//4\n"
                      "usemtl green\n"
                      "f 7//5 8//5 4//5\n"
                      "f 1//6 4//6 8//6\n"
                      "usemtl yellow\n"
                      "f 1//1 2//1 4//1\n"
                      "f 5//2 8//2 6//2\n"
                      "usemtl cyan\n"
                      "f 2//3 1//3 6//3\n"
                      "f 3//4 2//4 7//4\n"
                      "usemtl magenta\n"
                      "f 3//5 7//5 4//5\n"
                      "f 5//6 1//6 8//6\n");
  WavefrontImporter importer(ss);
  importer.parse();

  auto faces = importer.get_faces();
  
  Face face = faces[11];
  Color color = face.material->ambient;
  ASSERT_NEAR(1.0f, color[0], 0.00001f);
  ASSERT_NEAR(0.0f, color[1], 0.00001f);
  ASSERT_NEAR(1.0f, color[2], 0.00001f);
}
TEST(WAVEFRONT_IMPORTER, ParseFromFile) {
  std::fstream fs( "cube.obj" );  
  WavefrontImporter importer(fs);               
  importer.parse();                                       
  std::map<std::string, Material> materials = importer.get_materials();
  
  ASSERT_EQ(8, importer.get_vertices().size());  
  ASSERT_EQ(6, importer.get_normals().size());  
  ASSERT_EQ(12, importer.get_faces().size());  
  ASSERT_EQ(6, materials.size());
  Color color = materials["cyan"].ambient;
  ASSERT_NEAR(0.0f, color[0], 0.00001f);
  ASSERT_NEAR(1.0f, color[1], 0.00001f);
  ASSERT_NEAR(1.0f, color[2], 0.00001f);
}
}




#ifndef GEOMETRY_H
#define GEOMETRY_H


#include "math.h"
#include <iostream>
#include <vector>

// contains geometric shapes and related stuff, like spheres, triangles, intersection algorithms.


template <class FLOAT, size_t N>
struct Ray {
  Vector<FLOAT,N> origin,
                  direction;
};

// collection of intersection specific values, like intersection point, normal etc
template <class FLOAT, size_t N>
struct Intersection_Context {
  FLOAT u,v, // may be used for U-V-Mapping, values depend on intersected object
        t;  // intersection = ray.origin + t * ray.direction
  Vector<FLOAT, N> normal{},  // the intersection normal pointing away from the surface
                   intersection{};
};


// calculates the refracted rays direction (transmission) for the rays direction und a surface normal
// refraction_index is the quotient of the outside and inside material density 
// returns true if transmission occurs
// return false if total internal reflection occurs (no transmission  in this case).
// normal and direction have to be normalized
template <class FLOAT, size_t N>
bool refract(FLOAT refraction_index, Vector<FLOAT, N> normal, Vector<FLOAT, N> direction, Vector<FLOAT, N> & transmission);



/*
 a axis aligned bounding box (aabb) with a center point
  the half of the boxes edge length (for each dimension)


              half_edge_length[0]
             |           |
  +----------------------+--
  |                      |
  |                      | half_edge_length[1]
  |                      |
  |                      |
  |          o           |--
  |         center       |
  |                      |
  |                      |
  |                      |
  +----------------------+
*/
template <class FLOAT, size_t N>
class AxisAlignedBoundingBox {
protected:
  Vector<FLOAT,N> center,
                  half_edge_length;
public:
  AxisAlignedBoundingBox(Vector<FLOAT,N> center, Vector<FLOAT,N> half_edge_length);
  bool intersects(AxisAlignedBoundingBox<FLOAT,N> aabb) const;

  // checks if this aabb is intersected by the given ray
  bool intersects(Ray<FLOAT,N> ray) const;

  // checks if an intersection exists with an aabb moving in the given direction
  bool intersects(AxisAlignedBoundingBox<FLOAT,N> aabb, Vector<FLOAT, N> direction) const;
  
  // checks if an intersection exists with an aabb moving in the given direction
  // returns the normal (length not normalized) of the face that had been hit, or the null vector 
  // if no intersections occured
  Vector<FLOAT, N> sweep_intersects(AxisAlignedBoundingBox<FLOAT,N> aabb, Vector<FLOAT, N> direction) const;
};

// a sphere with a center and a radius
template <class FLOAT, size_t N>
class Sphere {
protected:
  Vector<FLOAT,N> center;
  FLOAT radius;
public:
  Sphere(Vector<FLOAT,N> center, FLOAT radius);
  
  // returns true iff the given ray intersects this sphere
  // context.intersection is set to the intersection point,
  // context.normal is set to the intersection normal facing away from the surface
  // context.t is set to the value with: ray.origin + t * ray.direction == intersection
  bool intersects(const Ray<FLOAT, N> &ray, Intersection_Context<FLOAT, N> & context) const;

  // returns a value t such that ray.origin + t * ray.direction is the intersection point
  // t is zero if no intersection occured
  FLOAT intersects(const Ray<FLOAT, N> &ray) const;

  // returns true iff this Sphere intersects with the given sphere
  /*
  bool intersects(Sphere<FLOAT, N> sphere) const;
  */
  
  // returns true iff the given point is inside this Sphere or on its surface
  /*
  bool inside(const Vector<FLOAT, N> p) const;
  */
};

template <class FLOAT, size_t N>
class Triangle {
protected:
  Vector<FLOAT, N> a, b, c;
  Vector<FLOAT, N> na, nb, nc;  // normal vectors for each point
public:
  // creates a triangle with the given edge points a,b,c
  // the normals point away from the surface given by clockwise orientation of a,b,c
  Triangle(Vector<FLOAT, N> a, Vector<FLOAT, N> b, Vector<FLOAT, N> c);

  // creates a triangle with the given edge points a,b,c 
  // the normals of the edges are set to the given normal vector
  Triangle(Vector<FLOAT, N> a, Vector<FLOAT, N> b, Vector<FLOAT, N> c, Vector<FLOAT, N> normal);

  // creates a triangle with the given edge points a,b,c 
  // the normal of each edge a,b, and c are set to na, nb, and nc
  Triangle(Vector<FLOAT, N> a, Vector<FLOAT, N> b, Vector<FLOAT, N> c, Vector<FLOAT, N> na, Vector<FLOAT, N> nb, Vector<FLOAT, N> nc);

  // returns true if this Triangle intersects the given ray
  // if an intersection occured, than intersection is set to the intersection point
  //   u and v are set to the barycentric coordinates of the intersection
  //   t is set to a value with intersection = ray.origin + t * ray.direction
  //   normal points away from the surface (clockwise order of a,b, and c)
  bool intersects(const Ray<FLOAT, N> &ray, Vector<FLOAT, N> & normal, Vector<FLOAT, N> & intersection, FLOAT & u, FLOAT & v, FLOAT & t) const;

  // returns true if this Triangle intersects the given ray
  // if an intersection occured, than context.intersection is set to the intersection point
  //   context.u and context.v are set to the barycentric coordinates of the intersection
  //   context.t is set to a value with intersection = ray.origin + t * ray.direction
  //   context.normal points away from the surface (clockwise order of a,b, and c)
  bool intersects(const Ray<FLOAT, N> &ray, Intersection_Context<FLOAT, N> & context) const;
};


typedef Ray<float, 2u> Ray2df;
typedef Ray<float, 3u> Ray3df;

typedef AxisAlignedBoundingBox<float, 2u> AABB2df;
typedef AxisAlignedBoundingBox<float, 3u> AABB3df;

typedef Sphere<float, 2u> Sphere2df;
typedef Sphere<float, 3u> Sphere3df;

typedef Triangle<float, 3u> Triangle3df;


#endif

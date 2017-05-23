
class Sphere : public GraphicsObj {
    public:
        double radius;
        Sphere(vec3 * position, double radius);
        bool rayIntersection(vec3 origin, vec3 direction); // Check to see if a given ray intersects the sphere
        vec3 calculateSurfaceNormal(vec3 surface_pos); // Calculate the normal vector to the object at the given position 
        vec3 calculateRayIntersection(vec3 origin, vec3 direction); // Calculate the position of the intersection of a ray and a graphics object
};
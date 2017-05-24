class Plane : public GraphicsObj {
    public:
        vec3 * normal;
        Plane(vec3 * position, vec3 * normal);
        bool rayIntersection(vec3 origin, vec3 direction); // Check to see if a given ray intersects the sphere
        vec3 calculateSurfaceNormal(vec3 surface_pos); // Calculate the normal vector to the object at the given position 
        vec3 calculateRayIntersection(vec3 origin, vec3 direction); // Calculate the position of the intersection of a ray and a graphics object
        COLOR getSurfaceColor(vec3 surface_pos);
};
class GraphicsObj {
    public:
        vec3 * position;
        GraphicsObj(vec3 * position); // set the object to have the given postion
        double calculateDistanceFrom(vec3 * point); // Calculates the distance from the point to the object
        virtual bool rayIntersection(vec3 origin, vec3 direction) = 0; // Check to see if a given ray intersects the sphere
        virtual vec3 calculateSurfaceNormal(vec3 surface_pos) = 0; // Calculate the normal vector to the object at the given position 
        virtual vec3 calculateRayIntersection(vec3 origin, vec3 direction) = 0; // Calculate the position of the intersection of a ray and a graphics object
};
class vec3 {
    public:
        double x;
        double y;
        double z;
        vec3(double x, double y, double z);
        double magnitude();
        static double dot(vec3 a, vec3 b);
        static vec3 cross(vec3 a, vec3 b);
        static vec3 normalize(vec3 a);
        vec3 operator+ (const vec3& param);
        vec3 operator- (const vec3& param);
        vec3 operator* (const double& param);
};

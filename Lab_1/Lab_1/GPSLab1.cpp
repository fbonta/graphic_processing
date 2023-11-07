//
//  GPSLab1.cpp
//
//  Copyright © 2017 CGIS. All rights reserved.
//

#include "GPSLab1.hpp"

namespace gps {
    glm::vec4 TransformPoint(const glm::vec4 &point)
    {
        glm::mat4 transformationMatrix(1.0f);

        transformationMatrix = rotate(transformationMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        transformationMatrix = translate(transformationMatrix, glm::vec3(2.0f, 0.0f, 1.0f));

        return transformationMatrix * point;
    }
    
    float ComputeAngle(const glm::vec3 &v1, const glm::vec3 &v2)
    {
        return glm::degrees(glm::acos(dot(v1, v2) / (length(v1) * length(v2))));
    }

    /*
     *https://stackoverflow.com/questions/471962/how-do-i-efficiently-determine-if-a-polygon-is-convex-non-convex-or-complex
     */
    bool IsConvex(const std::vector<glm::vec2> &vertices)
    {
        float sign = 0.0f;
        const size_t n = vertices.size();

        for(size_t i = 0 ; i < n; ++i)
        {
	        const float dx1 = vertices[(i + 1) % n].x - vertices[(i) % n].x;
            const float dx2 = vertices[(i + 2) % n].x - vertices[(i + 1) % n].x;
            const float dy1 = vertices[(i + 1) % n].y - vertices[(i) % n].y;
            const float dy2 = vertices[(i + 2) % n].y - vertices[(i + 1) % n].y;

            const float z_cross_product = dx1 * dy2 - dx2 * dy1;

            if (sign == 0.0f)
            {
	            if (z_cross_product < 0)
	            {
                    sign = -1.0f;
	            }
                else
                {
                    sign = 1.0f;
                }
            }

            if (z_cross_product * sign < 0)
            {
                return false;
            }
        }

        return true;
    }


    std::vector<glm::vec2> ComputeNormals(const std::vector<glm::vec2> &vertices)
    {
        std::vector<glm::vec2> normalsList;

        for (int i = 0 ; i < vertices.size(); ++i)
        {
            glm::vec2 i_result = vertices[(i + 1) % vertices.size()] - vertices[(i) % vertices.size()];
            i_result = normalize(i_result);
            normalsList.emplace_back(i_result.y, -i_result.x );
        }

        return normalsList;
    }
}

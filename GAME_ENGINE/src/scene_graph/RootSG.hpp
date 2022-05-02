
#ifndef GAME_ENGINE_ROOTSG_HPP
#define GAME_ENGINE_ROOTSG_HPP


#include "NodeSG.hpp"

namespace scene_graph {
    /// Root of the %scene graph
    class RootSG : public ElementSG {
    public:
        /**
         * Constructor of the RootSG
         * @param shaders
         * @param name
         */
        explicit RootSG(Shaders *shaders);

        glm::mat4 get_matrix_recursive(TransformDirty* dirty,bool inverse) override;
    };
}

#endif //GAME_ENGINE_ROOTSG_HPP

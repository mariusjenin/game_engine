
#ifndef GAME_ENGINE_ROOTSG_HPP
#define GAME_ENGINE_ROOTSG_HPP


#include "NodeSG.hpp"

namespace scene_graph {
    /// Root of the %scene graph
    class RootSG : public ElementSG {
    public:
        glm::mat4 get_matrix_recursive(TransformDirty* dirty,bool inverse) override;
    };
}

#endif //GAME_ENGINE_ROOTSG_HPP

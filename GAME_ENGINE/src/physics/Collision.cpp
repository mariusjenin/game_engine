#include <sstream>
#include "Collision.hpp"

using namespace physics;

std::string Collision::to_string() {
    std::stringstream ss;
    ss << "colliding : " << colliding << "\n" <<
       "normal : " << normal[0] << " " << normal[1] << " " << normal[2] << "\n" <<
       "depth : " << depth << "\n" <<
       "contacts size : " << contacts.size() << "\n";
    return ss.str();
}

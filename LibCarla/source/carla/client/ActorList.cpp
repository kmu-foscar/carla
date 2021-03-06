// Copyright (c) 2017 Computer Vision Center (CVC) at the Universitat Autonoma
// de Barcelona (UAB).
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.

#include "carla/client/ActorList.h"

#include "carla/StringUtil.h"
#include "carla/client/detail/ActorFactory.h"

#include <iterator>

namespace carla {
namespace client {

  ActorList::ActorList(
      detail::EpisodeProxy episode,
      std::vector<rpc::Actor> actors)
    : _episode(std::move(episode)),
      _actors(std::make_move_iterator(actors.begin()), std::make_move_iterator(actors.end())) {}

  SharedPtr<Actor> ActorList::Find(actor_id_type const actor_id) const {
    for (auto &actor : _actors) {
      if (actor_id == actor.GetId()) {
        return actor.Get(_episode, shared_from_this());
      }
    }
    return nullptr;
  }

  ActorList ActorList::Filter(const std::string &wildcard_pattern) const {
    ActorList filtered{_episode, {}};
    for (auto &&actor : _actors) {
      if (StringUtil::Match(actor.GetTypeId(), wildcard_pattern)) {
        filtered._actors.push_back(actor);
      }
    }
    return filtered;
  }

} // namespace client
} // namespace carla

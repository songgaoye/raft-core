#ifndef RAFT_CORE_CLUSTER_CONFIG_H_
#define RAFT_CORE_CLUSTER_CONFIG_H_

#include "raft_core/peer_id.h"

#include <initializer_list>
#include <vector>

namespace raft_core {

class ClusterConfig {
 public:
  ClusterConfig() = default;
  ClusterConfig(std::initializer_list<PeerId> peers);

  // Returns false when peer is already present.
  bool add_peer(PeerId peer);
  bool contains(PeerId peer) const noexcept;
  std::size_t size() const noexcept;
  bool empty() const noexcept;

  const std::vector<PeerId>& peers() const noexcept;

 private:
  std::vector<PeerId> peers_;
};

}  // namespace raft_core

#endif  // RAFT_CORE_CLUSTER_CONFIG_H_

#include "raft_core/ballot.h"

#include <cassert>

namespace raft_core {
namespace {

const PeerId kPeer1(1);
const PeerId kPeer2(2);
const PeerId kPeer3(3);
const PeerId kPeer4(4);
const PeerId kUnknownPeer(99);

void test_cluster_config_rejects_duplicate_peer() {
  ClusterConfig config;
  assert(config.add_peer(kPeer1));
  assert(!config.add_peer(kPeer1));
  assert(config.size() == 1);
  assert(config.contains(kPeer1));
  assert(!config.contains(kPeer2));
}

void test_majority_of_three() {
  Ballot ballot(ClusterConfig{kPeer1, kPeer2, kPeer3});

  assert(!ballot.granted());
  ballot.grant(kPeer1);
  assert(!ballot.granted());
  ballot.grant(kPeer2);
  assert(ballot.granted());
}

void test_duplicate_and_unknown_grants_do_not_count_twice() {
  Ballot ballot(ClusterConfig{kPeer1, kPeer2, kPeer3});

  ballot.grant(kPeer1);
  ballot.grant(kPeer1);
  ballot.grant(kUnknownPeer);
  assert(!ballot.granted());

  ballot.grant(kPeer2);
  assert(ballot.granted());
}

void test_joint_consensus_requires_two_majorities() {
  const ClusterConfig current{kPeer1, kPeer2, kPeer3};
  const ClusterConfig old{kPeer1, kPeer2, kPeer3, kPeer4};
  Ballot ballot(current, &old);

  ballot.grant(kPeer1);
  ballot.grant(kPeer2);
  assert(!ballot.granted());

  ballot.grant(kPeer4);
  assert(ballot.granted());
}

}  // namespace
}  // namespace raft_core

int main() {
  using namespace raft_core;

  test_cluster_config_rejects_duplicate_peer();
  test_majority_of_three();
  test_duplicate_and_unknown_grants_do_not_count_twice();
  test_joint_consensus_requires_two_majorities();
}

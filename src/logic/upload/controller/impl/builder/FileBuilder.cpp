#include "FileBuilder.hpp"

#include <algorithm>
#include <cassert>
#include <fstream>

namespace svh::video::logic::upload::controller::impl::builder {

const std::string k_default_build_dir = "/tmp";

FileBuilder::FileBuilder(std::string file_name, std::size_t total_blocks_number,
                         std::string workspace_dir)
    : workspace_direction_(std::move(workspace_dir)),
      blocks_hashes_(total_blocks_number, std::string()),
      total_blocks_(total_blocks_number),
      file_name_(std::move(file_name)) {
  if (!std::filesystem::exists(workspace_direction_))
    throw std::logic_error("workspace directory not exists");
}

bool FileBuilder::is_ready() {
  auto hashes_lock = blocks_hashes_.SharedLock();
  auto count_received_blocks =
      std::count_if(hashes_lock->begin(), hashes_lock->begin(),
                    [](const auto& hash) { return !hash.empty(); });
  return static_cast<std::size_t>(count_received_blocks) == total_blocks_;
}

std::string FileBuilder::get_name() const { return file_name_; }

void FileBuilder::push_block(std::size_t idx, std::string_view value) {
  auto block_hash = get_hash_if_need_write(idx, value);
  if (!block_hash) return;

  write_block(idx, value);

  update_block_hash(idx, std::move(*block_hash));
}

void FileBuilder::build(std::string path) {
  // TODO: make one file from parts
}

std::optional<std::string> FileBuilder::get_hash_if_need_write(
    std::size_t idx, std::string_view value) {
  // check, that idx in range [1, total block number]
  assert((idx >= 1u) && (idx <= total_blocks_));

  auto new_sum = userver::crypto::hash::Sha256(value);
  auto hash = blocks_hashes_.SharedLock();

  auto index = idx - 1u;
  const auto& stored_sum = (*hash)[index];

  if (stored_sum != new_sum)
    return std::make_optional<std::string>(std::move(new_sum));
  else
    return std::nullopt;
}

void FileBuilder::write_block(std::size_t idx, std::string_view value) {
  auto block_name = file_name_ + std::to_string(idx);
  std::filesystem::path file_block_path = workspace_direction_ / block_name;

  std::ofstream fout(file_block_path, std::ios::binary);
  if (!fout.is_open()) throw std::runtime_error("File open failed");

  fout.write(value.data(), value.size());
}

void FileBuilder::update_block_hash(std::size_t idx, std::string hash) {
  assert((idx >= 1u) && (idx <= total_blocks_));

  auto index = idx - 1u;
  auto hash_lock = blocks_hashes_.Lock();
  (*hash_lock)[index] = std::move(hash);
}

}  // namespace svh::video::logic::upload::controller::impl::builder
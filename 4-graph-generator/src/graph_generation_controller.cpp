#include <mutex>

#include "graph_generation_controller.hpp"

namespace uni_course_cpp {
namespace {
static const int kMaxThreadsCount = std::thread::hardware_concurrency();
};

void GraphGenerationController::generate(
    const GenStartedCallback& gen_started_callback,
    const GenFinishedCallback& gen_finished_callback) {
  std::mutex callback_mutex;

  for (int i = 0; i < graphs_count_; i++) {
    jobs_.emplace_back([this, &callback_mutex, &gen_started_callback,
                        &gen_finished_callback, i]() {
      {
        const std::lock_guard lock(callback_mutex);
        gen_started_callback(i);
      }

      auto graph = graph_generator_.generate();

      {
        const std::lock_guard lock(callback_mutex);
        gen_finished_callback(i, std::move(graph));
      }
    });
  }

  std::mutex jobs_mutex;
  Worker::GetJobCallback get_job_callback =
      [this, &jobs_mutex]() -> std::optional<JobCallback> {
    const std::lock_guard lock(jobs_mutex);
    if (jobs_.size()) {
      auto job = jobs_.front();
      jobs_.pop_front();
      return job;
    }
    return std::nullopt;
  };

  const auto threads_count = std::min(kMaxThreadsCount, threads_count_);
  for (int i = 0; i < threads_count; i++) {
    workers_.emplace_back(get_job_callback);
  }

  for (auto& worker : workers_) {
    worker.start();
  }

  while (jobs_.size()) {
  }

  for (auto& worker : workers_) {
    worker.stop();
  }
}

void GraphGenerationController::Worker::start() {
  if (state_ == State::Working) {
    return;
  }

  state_ = State::Working;

  thread_ = std::thread([this]() {
    while (true) {
      if (state_ == State::ShouldTerminate) {
        return;
      }

      const auto job_optional = get_job_callback_();
      if (job_optional.has_value()) {
        const auto& job = job_optional.value();
        job();
      }
    }
  });
}

void GraphGenerationController::Worker::stop() {
  if (state_ == State::Working) {
    state_ = State::ShouldTerminate;
    thread_.join();
  }
}

GraphGenerationController::Worker::~Worker() {
  stop();
}
};  // namespace uni_course_cpp

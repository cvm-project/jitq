#include "aws.hpp"

#include <aws/core/Aws.h>
#include <aws/core/utils/logging/AWSLogging.h>
#include <aws/core/utils/logging/ConsoleLogSystem.h>

namespace runtime {
namespace aws {

struct AwsApiHandle {
    explicit AwsApiHandle(Aws::SDKOptions&& options)
        : options_(std::move(options)) {
        Aws::InitAPI(options_);
    }
    AwsApiHandle(const AwsApiHandle&) = delete;
    AwsApiHandle(AwsApiHandle&&) = delete;
    AwsApiHandle& operator=(const AwsApiHandle&) = delete;
    AwsApiHandle& operator=(AwsApiHandle&&) = delete;

    ~AwsApiHandle() { Aws::ShutdownAPI(options_); }

private:
    Aws::SDKOptions options_;
};

void EnsureApiInitialized() {
    static bool is_initialized = false;
    if (is_initialized) return;
    is_initialized = true;

    Aws::SDKOptions options;
    options.loggingOptions.logLevel = Aws::Utils::Logging::LogLevel::Warn;
    options.loggingOptions.logger_create_fn = [=]() {
        return std::shared_ptr<Aws::Utils::Logging::LogSystemInterface>(
                new Aws::Utils::Logging::ConsoleLogSystem(
                        options.loggingOptions.logLevel));
    };

    static AwsApiHandle handle(std::move(options));
}

}  // namespace aws
}  // namespace runtime
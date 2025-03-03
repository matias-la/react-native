/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#import <Foundation/Foundation.h>

#import <React/RCTDefines.h>

/*
 * Defined in RCTUtils.m
 */
RCT_EXTERN BOOL RCTIsMainQueue(void);

/**
 * This is the main assert macro that you should use. Asserts should be compiled out
 * in production builds. You can customize the assert behaviour by setting a custom
 * assert handler through `RCTSetAssertFunction`.
 */
#ifndef NS_BLOCK_ASSERTIONS
#define RCTAssert(condition, ...)                                                                    \
  do {                                                                                               \
    if ((condition) == 0) {                                                                          \
      _RCTAssertFormat(#condition, __FILE__, __LINE__, __func__, __VA_ARGS__);                       \
      if (RCT_NSASSERT) {                                                                            \
        [[NSAssertionHandler currentHandler] handleFailureInFunction:(NSString *_Nonnull)@(__func__) \
                                                                file:(NSString *_Nonnull)@(__FILE__) \
                                                          lineNumber:__LINE__                        \
                                                         description:__VA_ARGS__];                   \
      }                                                                                              \
    }                                                                                                \
  } while (false)
#else
#define RCTAssert(condition, ...) \
  do {                            \
  } while (false)
#endif
RCT_EXTERN void _RCTAssertFormat(const char *, const char *, int, const char *, NSString *, ...)
    NS_FORMAT_FUNCTION(5, 6);

/**
 * Report a fatal condition when executing. These calls will _NOT_ be compiled out
 * in production, and crash the app by default. You can customize the fatal behaviour
 * by setting a custom fatal handler through `RCTSetFatalHandler` and
 * `RCTSetFatalExceptionHandler`.
 */
RCT_EXTERN void RCTFatal(NSError *error);
RCT_EXTERN void RCTFatalException(NSException *exception);

/**
 * The default error domain to be used for React errors.
 */
RCT_EXTERN NSString *const RCTErrorDomain;

/**
 * JS Stack trace provided as part of an NSError's userInfo
 */
RCT_EXTERN NSString *const RCTJSStackTraceKey;

/**
 * Raw JS Stack trace string provided as part of an NSError's userInfo
 */
RCT_EXTERN NSString *const RCTJSRawStackTraceKey;

/**
 * Objective-C stack trace string provided as part of an NSError's userInfo
 */
RCT_EXTERN NSString *const RCTObjCStackTraceKey;

/**
 * Name of fatal exceptions generated by RCTFatal
 */
RCT_EXTERN NSString *const RCTFatalExceptionName;

/**
 * A block signature to be used for custom assertion handling.
 */
typedef void (^RCTAssertFunction)(
    NSString *condition,
    NSString *fileName,
    NSNumber *lineNumber,
    NSString *function,
    NSString *message);

typedef void (^RCTFatalHandler)(NSError *error);
typedef void (^RCTFatalExceptionHandler)(NSException *exception);

/**
 * Convenience macro for asserting that a parameter is non-nil/non-zero.
 */
#define RCTAssertParam(name) RCTAssert(name, @"'%s' is a required parameter", #name)

/**
 * Convenience macro for asserting that we're running on main queue.
 */
#define RCTAssertMainQueue() RCTAssert(RCTIsMainQueue(), @"This function must be called on the main queue")

/**
 * Convenience macro for asserting that we're running off the main queue.
 */
#define RCTAssertNotMainQueue() RCTAssert(!RCTIsMainQueue(), @"This function must not be called on the main queue")

/**
 * These methods get and set the current assert function called by the RCTAssert
 * macros. You can use these to replace the standard behavior with custom assert
 * functionality.
 */
RCT_EXTERN void RCTSetAssertFunction(RCTAssertFunction assertFunction);
RCT_EXTERN RCTAssertFunction RCTGetAssertFunction(void);

/**
 * This appends additional code to the existing assert function, without
 * replacing the existing functionality. Useful if you just want to forward
 * assert info to an extra service without changing the default behavior.
 */
RCT_EXTERN void RCTAddAssertFunction(RCTAssertFunction assertFunction);

/**
 * This method temporarily overrides the assert function while performing the
 * specified block. This is useful for testing purposes (to detect if a given
 * function asserts something) or to suppress or override assertions temporarily.
 */
RCT_EXTERN void RCTPerformBlockWithAssertFunction(void (^block)(void), RCTAssertFunction assertFunction);

/**
 * These methods get and set the current fatal handler called by the `RCTFatal`
 * and `RCTFatalException` methods.
 */
RCT_EXTERN void RCTSetFatalHandler(RCTFatalHandler fatalHandler);
RCT_EXTERN RCTFatalHandler RCTGetFatalHandler(void);
RCT_EXTERN void RCTSetFatalExceptionHandler(RCTFatalExceptionHandler fatalExceptionHandler);
RCT_EXTERN RCTFatalExceptionHandler RCTGetFatalExceptionHandler(void);

/**
 * Get the current thread's name (or the current queue, if in debug mode)
 */
RCT_EXTERN NSString *RCTCurrentThreadName(void);

/**
 * Helper to get generate exception message from NSError
 */
RCT_EXTERN NSString *
RCTFormatError(NSString *message, NSArray<NSDictionary<NSString *, id> *> *stacktrace, NSUInteger maxMessageLength);

/**
 * Formats a JS stack trace for logging.
 */
RCT_EXTERN NSString *RCTFormatStackTrace(NSArray<NSDictionary<NSString *, id> *> *stackTrace);

/**
 * Convenience macro to assert which thread is currently running (DEBUG mode only)
 */
#if DEBUG

#define RCTAssertThread(thread, format...)                                                                            \
  _Pragma("clang diagnostic push") _Pragma("clang diagnostic ignored \"-Wdeprecated-declarations\"") RCTAssert(       \
      [(id)thread isKindOfClass:[NSString class]]       ? [RCTCurrentThreadName() isEqualToString:(NSString *)thread] \
          : [(id)thread isKindOfClass:[NSThread class]] ? [NSThread currentThread] == (NSThread *)thread              \
                                                        : dispatch_get_current_queue() == (dispatch_queue_t)thread,   \
      format);                                                                                                        \
  _Pragma("clang diagnostic pop")

#else

#define RCTAssertThread(thread, format...) \
  do {                                     \
  } while (0)

#endif

/**
 * Controls for ensuring the new architecture runtime assumption holds.
 * Note: this is work in progress.
 */

// Enable reporting of any violation related to the new React Native architecture.
// If RCT_NEW_ARCHITECTURE is defined, it is already enabled by default, otherwise, no violation will be
// reported until enabled.
// Note: enabling this at runtime is not early enough to report issues within ObjC class +load execution.
__attribute__((used)) RCT_EXTERN void RCTEnableNewArchitectureViolationReporting(BOOL enabled);

// When reporting is enabled, trigger an assertion.
__attribute__((used)) RCT_EXTERN void RCTEnforceNotAllowedForNewArchitecture(id context, NSString *extra);
// When reporting is enabled, trigger an error but do not crash. Use this to prepare a specific callsite
// for stricter enforcement. When ready, switch it to use the variant above.
__attribute__((used)) RCT_EXTERN void RCTErrorNotAllowedForNewArchitecture(id context, NSString *extra);
// When reporting is enabled, log an message. When ready, switch it to use the variant above.
__attribute__((used)) RCT_EXTERN void RCTLogNotAllowedForNewArchitecture(id context, NSString *extra);

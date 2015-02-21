
#import <Foundation/Foundation.h>
#import <objc/runtime.h>


@interface GSRTMethod : NSObject
{
}

+ (id)methodWithObjCMethod: (Method)method;
+ (id)methodWithSelector: (SEL)sel implementation: (IMP)imp signature: (NSString *)signature;

- (id)initWithObjCMethod: (Method)method;
- (id)initWithSelector: (SEL)sel implementation: (IMP)imp signature: (NSString *)signature;

- (SEL)selector;
- (NSString *)selectorName;
- (IMP)implementation;
- (NSString *)signature;

// for ObjC method instances, sets the underlying implementation
// for selector/implementation/signature instances, just changes the pointer
- (void)setImplementation: (IMP)newImp;

// easy sending of arbitrary methods with arbitrary arguments
// a simpler alternative to NSInvocation etc.
// for simple cases where the return type is an id, use sendToTarget:
// for others, use the returnValue: variant and pass a pointer to storage
// (you can pass NULL if you don't care about the return value)
// all arguments MUST BE WRAPPED in RTARG, e.g.:
// [method sendToTarget: target, RTARG(arg1), RTARG(arg2)]
#define GSRT_ARG_MAGIC_COOKIE 0xdeadbeef
#define GSRTARG(expr) RT_ARG_MAGIC_COOKIE, @encode(__typeof__(expr)), (__typeof__(expr) []){ expr }
- (id)sendToTarget: (id)target, ...;
- (void)returnValue: (void *)retPtr sendToTarget: (id)target, ...;

@end

@interface NSObject (GSRTMethodSendingAdditions)

- (id)gsrt_sendMethod: (GSRTMethod *)method, ...;
- (void)gsrt_returnValue:(void *)retPtr sendMethod: (GSRTMethod *)method, ...;

- (id)gsrt_sendSelector: (SEL)sel, ...;
- (void)gsrt_returnValue:(void *)retPtr sendSelector: (SEL)sel, ...;

@end

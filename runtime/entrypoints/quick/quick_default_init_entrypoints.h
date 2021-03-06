/*
 * Copyright (C) 2021 Paschalis Mpeis
 * Copyright (C) 2016 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ART_RUNTIME_ENTRYPOINTS_QUICK_QUICK_DEFAULT_INIT_ENTRYPOINTS_H_
#define ART_RUNTIME_ENTRYPOINTS_QUICK_QUICK_DEFAULT_INIT_ENTRYPOINTS_H_

#include "base/logging.h"  // FOR VLOG_IS_ON.
#include "entrypoints/jni/jni_entrypoints.h"
#include "entrypoints/runtime_asm_entrypoints.h"
#include "quick_alloc_entrypoints.h"
#include "quick_default_externs.h"
#include "quick_entrypoints.h"

#include "mcr_rt/opt_interface.h"

namespace art {

extern "C" void art_quick_invoke_stub(ArtMethod*, uint32_t*, uint32_t, Thread*, JValue*,
                                      const char*);
extern "C" void art_quick_invoke_static_stub(ArtMethod*, uint32_t*, uint32_t, Thread*, JValue*,
                                             const char*);

static void DefaultInitEntryPoints(JniEntryPoints* jpoints, QuickEntryPoints* qpoints) {
  // JNI
  jpoints->pDlsymLookup = art_jni_dlsym_lookup_stub;

  // Alloc
  ResetQuickAllocEntryPoints(qpoints, /* is_marking= */ true);

  // Resolution and initialization
  qpoints->pInitializeStaticStorage = art_quick_initialize_static_storage;
  qpoints->pResolveTypeAndVerifyAccess = art_quick_resolve_type_and_verify_access;
  qpoints->pResolveType = art_quick_resolve_type;
  qpoints->pResolveMethodHandle = art_quick_resolve_method_handle;
  qpoints->pResolveMethodType = art_quick_resolve_method_type;
  qpoints->pResolveString = art_quick_resolve_string;

  // Field
  qpoints->pSet8Instance = art_quick_set8_instance;
  qpoints->pSet8Static = art_quick_set8_static;
  qpoints->pSet16Instance = art_quick_set16_instance;
  qpoints->pSet16Static = art_quick_set16_static;
  qpoints->pSet32Instance = art_quick_set32_instance;
  qpoints->pSet32Static = art_quick_set32_static;
  qpoints->pSet64Instance = art_quick_set64_instance;
  qpoints->pSet64Static = art_quick_set64_static;
  qpoints->pSetObjInstance = art_quick_set_obj_instance;
  qpoints->pSetObjStatic = art_quick_set_obj_static;
  qpoints->pGetByteInstance = art_quick_get_byte_instance;
  qpoints->pGetBooleanInstance = art_quick_get_boolean_instance;
  qpoints->pGetShortInstance = art_quick_get_short_instance;
  qpoints->pGetCharInstance = art_quick_get_char_instance;
  qpoints->pGet32Instance = art_quick_get32_instance;
  qpoints->pGet64Instance = art_quick_get64_instance;
  qpoints->pGetObjInstance = art_quick_get_obj_instance;
  qpoints->pGetByteStatic = art_quick_get_byte_static;
  qpoints->pGetBooleanStatic = art_quick_get_boolean_static;
  qpoints->pGetShortStatic = art_quick_get_short_static;
  qpoints->pGetCharStatic = art_quick_get_char_static;
  qpoints->pGet32Static = art_quick_get32_static;
  qpoints->pGet64Static = art_quick_get64_static;
  qpoints->pGetObjStatic = art_quick_get_obj_static;

  // Array
  qpoints->pAputObject = art_quick_aput_obj;

  // JNI
  qpoints->pJniMethodStart = JniMethodStart;
  qpoints->pJniMethodFastStart = JniMethodFastStart;
  qpoints->pJniMethodStartSynchronized = JniMethodStartSynchronized;
  qpoints->pJniMethodEnd = JniMethodEnd;
  qpoints->pJniMethodEndSynchronized = JniMethodEndSynchronized;
  qpoints->pJniMethodEndWithReference = JniMethodEndWithReference;
  qpoints->pJniMethodFastEndWithReference = JniMethodFastEndWithReference;
  qpoints->pJniMethodEndWithReferenceSynchronized = JniMethodEndWithReferenceSynchronized;
  qpoints->pJniMethodFastEnd = JniMethodFastEnd;
  qpoints->pQuickGenericJniTrampoline = art_quick_generic_jni_trampoline;

  // Locks
  if (UNLIKELY(VLOG_IS_ON(systrace_lock_logging))) {
    qpoints->pLockObject = art_quick_lock_object_no_inline;
    qpoints->pUnlockObject = art_quick_unlock_object_no_inline;
  } else {
    qpoints->pLockObject = art_quick_lock_object;
    qpoints->pUnlockObject = art_quick_unlock_object;
  }

  // Invocation
  qpoints->pQuickImtConflictTrampoline = art_quick_imt_conflict_trampoline;
  qpoints->pQuickResolutionTrampoline = art_quick_resolution_trampoline;
  qpoints->pQuickToInterpreterBridge = art_quick_to_interpreter_bridge;
  qpoints->pInvokeDirectTrampolineWithAccessCheck =
      art_quick_invoke_direct_trampoline_with_access_check;
  qpoints->pInvokeInterfaceTrampolineWithAccessCheck =
      art_quick_invoke_interface_trampoline_with_access_check;
  qpoints->pInvokeStaticTrampolineWithAccessCheck =
      art_quick_invoke_static_trampoline_with_access_check;
  qpoints->pInvokeSuperTrampolineWithAccessCheck =
      art_quick_invoke_super_trampoline_with_access_check;
  qpoints->pInvokeVirtualTrampolineWithAccessCheck =
      art_quick_invoke_virtual_trampoline_with_access_check;
  qpoints->pInvokePolymorphic = art_quick_invoke_polymorphic;
  qpoints->pInvokeCustom = art_quick_invoke_custom;

  // Thread
  qpoints->pTestSuspend = art_quick_test_suspend;

  // Throws
  qpoints->pDeliverException = art_quick_deliver_exception;
  qpoints->pThrowArrayBounds = art_quick_throw_array_bounds;
  qpoints->pThrowDivZero = art_quick_throw_div_zero;
  qpoints->pThrowNullPointer = art_quick_throw_null_pointer_exception;
  qpoints->pThrowStackOverflow = art_quick_throw_stack_overflow;
  qpoints->pThrowStringBounds = art_quick_throw_string_bounds;
  // Deoptimize
  qpoints->pDeoptimize = art_quick_deoptimize_from_compiled_code;

#ifdef ART_MCR  // LLVM - Resolution and initialization
  qpoints->pLLVMReadBarrierSlow = art_llvm_read_barrier_slow;
  // class resolution
  qpoints->pLLVMResolveType = art_llvm_resolve_type;
  qpoints->pLLVMResolveTypeAndVerifyAccess = art_llvm_resolve_type_and_verify_access;
  qpoints->pLLVMResolveTypeInternal= art_llvm_resolve_type_internal;

  // Method resolution
  qpoints->pLLVMResolveInternalMethod = art_llvm_resolve_internal_method;
  qpoints->pLLVMResolveExternalMethod = art_llvm_resolve_external_method;
  qpoints->pLLVMResolveVirtualMethod = art_llvm_resolve_virtual_method;
  qpoints->pLLVMResolveInterfaceMethod = art_llvm_resolve_interface_method;
  qpoints->pLLVMResolveString = art_llvm_resolve_string;

  qpoints->pLLVMTestSuspend= art_llvm_test_suspend;

  qpoints->pLLVMJValueSetL= art_llvm_jvalue_setl;
  qpoints->pLLVMGetObjStatic = art_llvm_get_obj_static;
  qpoints->pLLVMGetObjInstance = art_llvm_get_obj_instance;

  // Invoke entrypoints
  qpoints->pLLVMInvokeQuickWrapper = art_llvm_invoke_quick;
  qpoints->pLLVMInvokeQuickStaticWrapper = art_llvm_invoke_quick_static;

  qpoints->pLLVMInvokeQuickDirect = art_quick_invoke_stub;
  qpoints->pLLVMInvokeQuickStaticDirect = art_quick_invoke_static_stub;

  // Managed Stack
  qpoints->pLLVMPushQuickFrame= art_llvm_push_quick_frame;
  qpoints->pLLVMPopQuickFrame= art_llvm_pop_quick_frame;
  qpoints->pLLVMClearTopOfStack= art_llvm_clear_top_of_stack;

  // Debug verification
  qpoints->pLLVMVerifyArtMethod= art_llvm_verify_art_method;
  qpoints->pLLVMVerifyArtClass= art_llvm_verify_art_class;
  qpoints->pLLVMVerifyArtObject= art_llvm_verify_art_object;
  qpoints->pLLVMVerifyStackFrameCurrent= llvm_verify_stack_frame_current;
  // mcr::OptimizingInterface::qpoints_=qpoints;
#endif
}

}  // namespace art

#endif  // ART_RUNTIME_ENTRYPOINTS_QUICK_QUICK_DEFAULT_INIT_ENTRYPOINTS_H_

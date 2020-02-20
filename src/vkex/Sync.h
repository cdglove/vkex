/*
 Copyright 2018-2019 Google Inc.
 
 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at
 
 http://www.apache.org/licenses/LICENSE-2.0
 
 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
*/

#ifndef __VKEX_SYNC_H__
#define __VKEX_SYNC_H__

#include <vkex/Config.h>
#include <vkex/Traits.h>
#include <vkex/VulkanUtil.h>

namespace vkex {

// =================================================================================================
// Event
// =================================================================================================

/** @struct EventCreateInfo 
 *
 */
struct EventCreateInfo {
  std::string         object_name;
  VkEventCreateFlags  flags;
};

/** @class IEvent
 *
 */ 
class CEvent : public IDeviceObject {
public:
  CEvent();
  ~CEvent();

  /** @fn operator VkEvent()
   *
   */
  operator VkEvent() const { 
    return m_vk_object; 
  }

  /** @fn GetVkObject
   *
   */
  VkEvent GetVkObject() const { 
    return m_vk_object; 
  }

private:
  friend class CDevice;
  friend class IObjectStorageFunctions;

  /** @fn InternalCreate
   *
   */
  vkex::Result InternalCreate(
    const vkex::EventCreateInfo&  create_info,
    const VkAllocationCallbacks*  p_allocator
  );

  /** @fn InternalDestroy
   *
   */
  vkex::Result InternalDestroy(const VkAllocationCallbacks* p_allocator);

private:
  vkex::EventCreateInfo m_create_info = {};
  VkEventCreateInfo     m_vk_create_info = {};
  VkEvent               m_vk_object = VK_NULL_HANDLE;
};

// =================================================================================================
// Fence
// =================================================================================================

/** @struct FenceCreateInfo 
 *
 */
struct FenceCreateInfo {
  std::string       object_name;
  FenceCreateFlags  flags;
};

/** @class IFence
 *
 */ 
class CFence : public IDeviceObject {
public:
  CFence();
  ~CFence();

  /** @fn GetDevice
   *
   */
  vkex::Device GetDevice() const { 
    return m_device; 
  } 

  /** @fn operator VkFence()
   *
   */
  operator VkFence() const { 
    return m_vk_object; 
  }

  /** @fn GetVkObject
   *
   */
  VkFence GetVkObject() const { 
    return m_vk_object; 
  }

  /** @fn ResetFence
   *
   */
  VkResult ResetFence();

  /** @fn GetFenceStatus
   *
   */
  VkResult GetFenceStatus();

  /** @fn WaitForFence
   *
   */
  VkResult WaitForFence(uint64_t timeout = UINT64_MAX);

  /** @fn WaitForAndResetFence
   *
   */
  VkResult WaitForAndResetFence(uint64_t timeout = UINT64_MAX);

private:
  friend class CDevice;
  friend class IObjectStorageFunctions;

  /** @fn InternalCreate
   *
   */
  vkex::Result InternalCreate(
    const vkex::FenceCreateInfo&  create_info,
    const VkAllocationCallbacks*  p_allocator
  );

  /** @fn InternalDestroy
   *
   */
  vkex::Result InternalDestroy(const VkAllocationCallbacks* p_allocator);

private:
  vkex::FenceCreateInfo m_create_info = {};
  VkFenceCreateInfo     m_vk_create_info = {};
  VkFence               m_vk_object = VK_NULL_HANDLE;
};

// =================================================================================================
// Semaphore
// =================================================================================================

/** @struct SemaphoreCreateInfo 
 *
 */
struct SemaphoreCreateInfo {
  std::string           object_name          = "";
  SemaphoreCreateFlags  create_flags         = 0;
  VkPipelineStageFlags  wait_dst_stage_mask  = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;

#if defined(VKEX_ENABLE_TIMELINE_SEMAPHORE)
  VkSemaphoreTypeKHR    semaphore_type       = VK_SEMAPHORE_TYPE_BINARY_KHR;
  uint64_t              initial_value        = 0; // NOTE: must be zero per Vulkan spec
#endif // defined(VKEX_ENABLE_TIMELINE_SEMAPHORE)
};

/** @class ISemaphore
 *
 */ 
class CSemaphore : public IDeviceObject {
public:
  CSemaphore();
  ~CSemaphore();

  /** @fn operator VkSemaphore()
   *
   */
  operator VkSemaphore() const { 
    return m_vk_object; 
  }

  /** @fn GetVkObject
   *
   */
  VkSemaphore GetVkObject() const { 
    return m_vk_object; 
  }

  /** @fn GetWaitDstStageMask
   *
   */
  VkPipelineStageFlags GetWaitDstStageMask() const;

  /** @fn SetWaitDstStageMask
   *
   */
  void SetWaitDstStageMask(VkPipelineStageFlags mask);

#if defined(VKEX_ENABLE_TIMELINE_SEMAPHORE)
  /** @fn GetSemaphoreType
   *
   */
  VkSemaphoreTypeKHR GetSemaphoreType() const {
    return m_create_info.semaphore_type;
  }

  /** @fn Signal
   *
   */
  VkResult Signal(uint64_t value);

  /** @fn Wait
   *
   */
  VkResult Wait(uint64_t value, uint64_t timeout = UINT64_MAX);
#endif // defined(VKEX_ENABLE_TIMELINE_SEMAPHORE)

private:
  friend class CDevice;
  friend class IObjectStorageFunctions;

  /** @fn InternalCreate
   *
   */
  vkex::Result InternalCreate(
    const vkex::SemaphoreCreateInfo&  create_info,
    const VkAllocationCallbacks*      p_allocator
  );

  /** @fn InternalDestroy
   *
   */
  vkex::Result InternalDestroy(const VkAllocationCallbacks* p_allocator);

private:
  vkex::SemaphoreCreateInfo    m_create_info = {};
  VkSemaphoreCreateInfo        m_vk_create_info = {};
  VkSemaphore                  m_vk_object = VK_NULL_HANDLE;

#if defined(VKEX_ENABLE_TIMELINE_SEMAPHORE)
  VkSemaphoreTypeCreateInfoKHR m_vk_type_create_info = {};
#endif // defined(VKEX_ENABLE_TIMELINE_SEMAPHORE)
};

} // namespace vkex

#endif // __VKEX_SYNC_H__

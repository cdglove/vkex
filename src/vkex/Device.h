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

#ifndef __VKEX_DEVICE_H__
#define __VKEX_DEVICE_H__

#include "vkex/Buffer.h"
#include "vkex/Command.h"
#include "vkex/Descriptor.h"
#include "vkex/Image.h"
#include "vkex/Pipeline.h"
#include "vkex/QueryPool.h"
#include "vkex/Queue.h"
#include "vkex/RenderPass.h"
#include "vkex/Sampler.h"
#include "vkex/Shader.h"
#include "vkex/Swapchain.h"
#include "vkex/Sync.h"
#include "vkex/Texture.h"
#include "vkex/Traits.h"
#include "vkex/View.h"

namespace vkex {
  
// =================================================================================================
// PhysicalDevice
// =================================================================================================

/** @struct PhysicalDeviceCreateInfo 
 *
 */
struct PhysicalDeviceCreateInfo {
  VkPhysicalDevice  vk_object;
};

/** @class IPhysicalDevice
 *
 */ 
class CPhysicalDevice {
public:
  CPhysicalDevice();
  ~CPhysicalDevice();

  /** @fn operator VkPhysicalDevice()
   *
   */
  operator VkPhysicalDevice() const { 
    return m_create_info.vk_object;
  }

  /** @fn GetVkObject
   *
   */
  VkPhysicalDevice GetVkObject() const {
    return m_create_info.vk_object;
  }

  /** @fn GetVkPhysicalDeviceProperties
   *
   */
  const VkPhysicalDeviceProperties2& GetPhysicalDeviceProperties() const {
    return m_vk_physical_device_properties;
  }

  /** @fn GetVendorId
   *
   */
  uint32_t GetVendorId() const {
    return m_vk_physical_device_properties.properties.vendorID;
  }

  /** @fn IsAMD
   *
   */
  bool IsAMD() const {
    bool is_same = (GetVendorId() == VKEX_IHV_VENDOR_ID_AMD);
    return is_same;
  }

  /** @fn IsIntel
   *
   */
  bool IsIntel() const {
    bool is_same = (GetVendorId() == VKEX_IHV_VENDOR_ID_INTEL);
    return is_same;
  }

  /** @fn IsNVidia
   *
   */
  bool IsNVidia() const {
    bool is_same = (GetVendorId() == VKEX_IHV_VENDOR_ID_NVIDIA);
    return is_same;
  }

  const VkPhysicalDeviceShaderCorePropertiesAMD& GetShaderCorePropertiesAMD() const {
    return m_vendor_properties.amd.shader_core_properties;
  }

  /** @fn GetVkPhysicalDeviceLimits
   *
   */
  const VkPhysicalDeviceLimits& GetPhysicalDeviceLimits() const {
    return m_vk_physical_device_properties.properties.limits;
  }  

  /** @fn GetVkPhysicalDeviceFeatures
   *
   */
  const VkPhysicalDeviceFeatures2& GetPhysicalDeviceFeatures() const {
    return m_vk_physical_device_features;
  }

  /** @fn GetVkApiVersion
   *
   */
  uint32_t GetApiVersion() const {
    return m_vk_physical_device_properties.properties.apiVersion;
  }

  /** @fn GetDeviceName()
   *
   */
  const char* GetDeviceName() const {
    return m_vk_physical_device_properties.properties.deviceName;
  }

  /** @fn GetDescriptiveName()
   *
   */
  const char* GetDescriptiveName() const {
    return m_descriptive_name.c_str();
  }

  /** @fn GetVkQueueFamilyProperties
   *
   */
  const std::vector<VkQueueFamilyProperties2>& GetQueueFamilyProperties() const {
    return m_vk_queue_family_properties;
  }

  /** @fn GetVkQueueFamilyProperties
   *
   */
  bool GetQueueFamilyProperties(
    uint32_t                  vk_queue_family_index, 
    VkQueueFamilyProperties2* p_vk_queue_family_properties
  ) const;

  /** @fn GetVkPhysicalDeviceMemoryProperties
   *
   */
  const VkPhysicalDeviceMemoryProperties& GetPhysicalDeviceMemoryProperties() const {
    return m_vk_physical_device_memory_properties.memoryProperties;
  }
  
  /** @fn SupportsPresent
   *
   */
  VkBool32 SupportsPresent(uint32_t queue_family_index, const vkex::DisplayInfo& display_info) const;

private:
  friend class CInstance;
  friend class IObjectStorageFunctions;

  /** @fn InternalCreate
   *
   */
  vkex::Result InternalCreate(
    const vkex::PhysicalDeviceCreateInfo& create_info,
    const VkAllocationCallbacks*          p_allocator
  );

  /** @fn InternalDestroy
   *
   */
  vkex::Result InternalDestroy(const VkAllocationCallbacks* p_allocator);

  /** @fn InitializeDescriptiveName
   * 
   */
  void InitializeVendorProperties();
  
  /** @fn SetInstance
   *
   */
  void SetInstance(vkex::Instance instance) {
    m_instance = instance;
  }

private:
  vkex::Instance                              m_instance = nullptr;
  PhysicalDeviceCreateInfo                    m_create_info = {};
  VkPhysicalDeviceProperties2                 m_vk_physical_device_properties = {};
  VkPhysicalDeviceFeatures2                   m_vk_physical_device_features = {};
  std::vector<VkQueueFamilyProperties2>       m_vk_queue_family_properties;
  VkPhysicalDeviceMemoryProperties2           m_vk_physical_device_memory_properties = {};

  struct {
    struct {
      VkPhysicalDeviceShaderCorePropertiesAMD shader_core_properties;
    } amd;
  } m_vendor_properties;

  mutable std::string                         m_descriptive_name;
};

// =================================================================================================
// PhysicalDeviceFeatures
// =================================================================================================
struct PhysicalDeviceFeatures 
{
  VkBool32 robustBufferAccess                      = VK_FALSE;
  VkBool32 fullDrawIndexUint32                     = VK_FALSE;
  VkBool32 imageCubeArray                          = VK_FALSE;
  VkBool32 independentBlend                        = VK_FALSE;
  VkBool32 geometryShader                          = VK_TRUE;
  VkBool32 tessellationShader                      = VK_TRUE;
  VkBool32 sampleRateShading                       = VK_FALSE;
  VkBool32 dualSrcBlend                            = VK_TRUE;
  VkBool32 logicOp                                 = VK_FALSE;
  VkBool32 multiDrawIndirect                       = VK_FALSE;
  VkBool32 drawIndirectFirstInstance               = VK_FALSE;
  VkBool32 depthClamp                              = VK_FALSE;
  VkBool32 depthBiasClamp                          = VK_FALSE;
  VkBool32 fillModeNonSolid                        = VK_FALSE;
  VkBool32 depthBounds                             = VK_FALSE;
  VkBool32 wideLines                               = VK_FALSE;
  VkBool32 largePoints                             = VK_FALSE;
  VkBool32 alphaToOne                              = VK_FALSE;
  VkBool32 multiViewport                           = VK_FALSE;
  VkBool32 samplerAnisotropy                       = VK_TRUE;
  VkBool32 textureCompressionETC2                  = VK_FALSE;
  VkBool32 textureCompressionASTC_LDR              = VK_FALSE;
  VkBool32 textureCompressionBC                    = VK_FALSE;
  VkBool32 occlusionQueryPrecise                   = VK_FALSE;
  VkBool32 pipelineStatisticsQuery                 = VK_TRUE;
  VkBool32 vertexPipelineStoresAndAtomics          = VK_FALSE;
  VkBool32 fragmentStoresAndAtomics                = VK_FALSE;
  VkBool32 shaderTessellationAndGeometryPointSize  = VK_FALSE;
  VkBool32 shaderImageGatherExtended               = VK_FALSE;
  VkBool32 shaderStorageImageExtendedFormats       = VK_FALSE;
  VkBool32 shaderStorageImageMultisample           = VK_FALSE;
  VkBool32 shaderStorageImageReadWithoutFormat     = VK_FALSE;
  VkBool32 shaderStorageImageWriteWithoutFormat    = VK_FALSE;
  VkBool32 shaderUniformBufferArrayDynamicIndexing = VK_FALSE;
  VkBool32 shaderSampledImageArrayDynamicIndexing  = VK_FALSE;
  VkBool32 shaderStorageBufferArrayDynamicIndexing = VK_FALSE;
  VkBool32 shaderStorageImageArrayDynamicIndexing  = VK_FALSE;
  VkBool32 shaderClipDistance                      = VK_FALSE;
  VkBool32 shaderCullDistance                      = VK_FALSE;
  VkBool32 shaderFloat64                           = VK_FALSE;
  VkBool32 shaderInt64                             = VK_FALSE;
  VkBool32 shaderInt16                             = VK_FALSE;
  VkBool32 shaderResourceResidency                 = VK_FALSE;
  VkBool32 shaderResourceMinLod                    = VK_FALSE;
  VkBool32 sparseBinding                           = VK_FALSE;
  VkBool32 sparseResidencyBuffer                   = VK_FALSE;
  VkBool32 sparseResidencyImage2D                  = VK_FALSE;
  VkBool32 sparseResidencyImage3D                  = VK_FALSE;
  VkBool32 sparseResidency2Samples                 = VK_FALSE;
  VkBool32 sparseResidency4Samples                 = VK_FALSE;
  VkBool32 sparseResidency8Samples                 = VK_FALSE;
  VkBool32 sparseResidency16Samples                = VK_FALSE;
  VkBool32 sparseResidencyAliased                  = VK_FALSE;
  VkBool32 variableMultisampleRate                 = VK_FALSE;
  VkBool32 inheritedQueries                        = VK_FALSE;
#if defined(VKEX_ENABLE_TIMELINE_SEMAPHORE)
  VkBool32 timelineSemaphore                       = VK_TRUE;
#endif
  operator VkPhysicalDeviceFeatures() const;
};

// =================================================================================================
// Device
// =================================================================================================

struct DeviceQueueCreateInfo {
  VkQueueFlagBits     queue_type;
  uint32_t            queue_family_index;
  uint32_t            queue_count;
  std::vector<float>  queue_priorities;
};

/** @struct DeviceCreateInfo 
 *
 */
struct DeviceCreateInfo {
  PhysicalDevice                     physical_device;
  std::vector<DeviceQueueCreateInfo> queue_create_infos;
  std::vector<std::string>           extensions;
  vkex::PhysicalDeviceFeatures       enabled_features;
};

/** @class IDevice
 *
 */ 
class CDevice
  : //public DeviceFunctionSet,
    protected IObjectStorageFunctions
{
public:
  CDevice();
  ~CDevice();

  /** @fn operator VkBuffer()
   *
   */
  operator VkDevice() const { 
    return m_vk_object; 
  }

  /** @fn GetVkObject
   *
   */
  VkDevice GetVkObject() const { 
    return m_vk_object; 
  }

  /** @fn GetInstance
   *
   */
  Instance GetInstance() const {
    return m_instance;
  }

  /** @fn GetPhysicalDevice
   *
   */
  PhysicalDevice GetPhysicalDevice() const {
    return m_create_info.physical_device;
  }

  /** @fn GetLoadedExtensions
   *
   */
  const std::vector<std::string> GetLoadedExtensions() const {
    return m_create_info.extensions;
  }

  /** @n GetDeviceName()
   *
   */
  const char* GetDeviceName() const {
    return m_create_info.physical_device->GetDeviceName();
  }

  /** @fn GetDescriptiveName()
   *
   */
  const char* GetDescriptiveName() const {
    return m_create_info.physical_device->GetDescriptiveName();
  }

  /** @fn GetVmaAllocator
   * 
   */
  VmaAllocator GetVmaAllocator() const {
    return m_vma_allocator;
  }

  /** @fn GetQueue
   * 
   */
  vkex::Result GetQueue(
    VkQueueFlagBits queue_type, 
    uint32_t        queue_family_index, 
    uint32_t        queue_index, 
    vkex::Queue*    p_queue
  ) const;

  /** @fn WaitIdle
   * 
   */
  VkResult WaitIdle();

  /** @fn CreateBuffer
   *
   */
  vkex::Result CreateBuffer(
    const vkex::BufferCreateInfo& create_info,
    vkex::Buffer*                 p_object,
    const VkAllocationCallbacks*  p_allocator = nullptr
  );

  /** @fn DestroyBuffer
   *
   */
  vkex::Result DestroyBuffer(
    vkex::Buffer                  object,
    const VkAllocationCallbacks*  p_allocator = nullptr
  );

  /** @fn CreateCommandPool
   *
   */
  vkex::Result CreateCommandPool(
    const vkex::CommandPoolCreateInfo&  create_info,
    vkex::CommandPool*                  p_object,
    const VkAllocationCallbacks*        p_allocator = nullptr
  );

  /** @fn DestroyCommandPool
   *
   */
  vkex::Result DestroyCommandPool(
    vkex::CommandPool             object,
    const VkAllocationCallbacks*  p_allocator = nullptr
  );

  /** @fn CreateComputePipeline
   *
   */
  vkex::Result CreateComputePipeline(
    const vkex::ComputePipelineCreateInfo&  create_info,
    vkex::ComputePipeline*                  p_object,
    const VkAllocationCallbacks*            p_allocator = nullptr
  );

  /** @fn DestroyComputePipeline
   *
   */
  vkex::Result DestroyComputePipeline(
    vkex::ComputePipeline         object,
    const VkAllocationCallbacks*  p_allocator = nullptr
  );

  /** @fn CreateConstantBuffer
   *
   */
  vkex::Result CreateConstantBuffer(
    const vkex::BufferCreateInfo& create_info,
    vkex::Buffer*                 p_object,
    const VkAllocationCallbacks*  p_allocator = nullptr
  );

  /** @fn DestroyConstantBuffer
   *
   */
  vkex::Result DestroyConstantBuffer(
    vkex::Buffer                  object,
    const VkAllocationCallbacks*  p_allocator = nullptr
  );

  /** @fn CreateDepthStencilView
   *
   */
  vkex::Result CreateDepthStencilView(
    const vkex::DepthStencilViewCreateInfo& create_info,
    vkex::DepthStencilView*                 p_object,
    const VkAllocationCallbacks*            p_allocator = nullptr
  );

  /** @fn DestroyDepthStencilView
   *
   */
  vkex::Result DestroyDepthStencilView(
    vkex::DepthStencilView          object,
    const VkAllocationCallbacks*    p_allocator = nullptr
  );

  /** @fn CreateDescriptorPool
   *
   */
  vkex::Result CreateDescriptorPool(
    const vkex::DescriptorPoolCreateInfo& create_info,
    vkex::DescriptorPool*                 p_object,
    const VkAllocationCallbacks*          p_allocator = nullptr
  );

  /** @fn DestroyDescriptorPool
   *
   */
  vkex::Result DestroyDescriptorPool(
    vkex::DescriptorPool          object,
    const VkAllocationCallbacks*  p_allocator = nullptr
  );

  /** @fn CreateDescriptorSetLayout
   *
   */
  vkex::Result CreateDescriptorSetLayout(
    const vkex::DescriptorSetLayoutCreateInfo&  create_info,
    vkex::DescriptorSetLayout*                  p_object,
    const VkAllocationCallbacks*                p_allocator = nullptr
  );

  /** @fn DestroyDescriptorSetLayout
   *
   */
  vkex::Result DestroyDescriptorSetLayout(
    vkex::DescriptorSetLayout     object,
    const VkAllocationCallbacks*  p_allocator = nullptr
  );

  /** @fn CreateDescriptorSetLayouts
   *
   */
  vkex::Result CreateDescriptorSetLayouts(
    const std::vector<vkex::DescriptorSetLayoutCreateInfo>& create_infos,
    std::vector<vkex::DescriptorSetLayout>*                 p_objects,
    const VkAllocationCallbacks*                            p_allocator = nullptr
  );

  /** @fn DestroyDescriptorSetLayouts
   *
   */
  vkex::Result DestroyDescriptorSetLayouts(
    const std::vector<vkex::DescriptorSetLayout>& objects,
    const VkAllocationCallbacks*                  p_allocator = nullptr
  );

  /** @fn CreateEvent
   *
   */
  vkex::Result CreateEvent(
    const vkex::EventCreateInfo&  create_info,
    vkex::Event*                  p_object,
    const VkAllocationCallbacks*  p_allocator = nullptr
  );

  /** @fn DestroyEvent
   *
   */
  vkex::Result DestroyEvent(
    vkex::Event                   object,
    const VkAllocationCallbacks*  p_allocator = nullptr
  );

  /** @fn CreateFence
   *
   */
  vkex::Result CreateFence(
    const vkex::FenceCreateInfo&  create_info,
    vkex::Fence*                  p_object,
    const VkAllocationCallbacks*  p_allocator = nullptr
  );

  /** @fn DestroyFence
   *
   */
  vkex::Result DestroyFence(
    vkex::Fence                   object,
    const VkAllocationCallbacks*  p_allocator = nullptr
  );

  /** @fn CreateIndexBuffer
   *
   */
  vkex::Result CreateIndexBuffer(
    const vkex::BufferCreateInfo& create_info,
    vkex::Buffer*                 p_object,
    const VkAllocationCallbacks*  p_allocator = nullptr
  );

  /** @fn DestroyIndexBuffer
   *
   */
  vkex::Result DestroyIndexBuffer(
    vkex::Buffer                  object,
    const VkAllocationCallbacks*  p_allocator = nullptr
  );

  /** @fn CreateIndirectBuffer
   *
   */
  vkex::Result CreateIndirectBuffer(
    const vkex::BufferCreateInfo& create_info,
    vkex::Buffer*                 p_object,
    const VkAllocationCallbacks*  p_allocator = nullptr
  );

  /** @fn DestroyIndirectBuffer
   *
   */
  vkex::Result DestroyIndirectBuffer(
    vkex::Buffer                  object,
    const VkAllocationCallbacks*  p_allocator = nullptr
  );

  /** @fn CreateGraphicsPipeline
   *
   */
  vkex::Result CreateGraphicsPipeline(
    const vkex::GraphicsPipelineCreateInfo& create_info,
    vkex::GraphicsPipeline*                 p_object,
    const VkAllocationCallbacks*            p_allocator = nullptr
  );

  /** @fn DestroyGraphicsPipeline
   *
   */
  vkex::Result DestroyGraphicsPipeline(
    vkex::GraphicsPipeline        object,
    const VkAllocationCallbacks*  p_allocator = nullptr
  );

  /** @fn CreateImage
   *
   */
  vkex::Result CreateImage(
    const vkex::ImageCreateInfo&  create_info,
    vkex::Image*                  p_object,
    const VkAllocationCallbacks*  p_allocator = nullptr
  );

  /** @fn DestroyImage
   *
   */
  vkex::Result DestroyImage(
    vkex::Image                   object,
    const VkAllocationCallbacks*  p_allocator = nullptr
  );

  /** @fn CreateImageView
   *
   */
  vkex::Result CreateImageView(
    const vkex::ImageViewCreateInfo&  create_info,
    vkex::ImageView*                  p_object,
    const VkAllocationCallbacks*      p_allocator = nullptr
  );

  /** @fn DestroyImageView
   *
   */
  vkex::Result DestroyImageView(
    vkex::ImageView               object,
    const VkAllocationCallbacks*  p_allocator = nullptr
  );

  /** @fn CreatePipelineCache
   *
   */
  vkex::Result CreatePipelineCache(
    const vkex::PipelineCacheCreateInfo&  create_info,
    vkex::PipelineCache*                  p_object,
    const VkAllocationCallbacks*          p_allocator = nullptr
  );

  /** @fn DestroyPipelineCache
   *
   */
  vkex::Result DestroyPipelineCache(
    vkex::PipelineCache           object,
    const VkAllocationCallbacks*  p_allocator = nullptr
  );

  /** @fn CreatePipelineLayout
   *
   */
  vkex::Result CreatePipelineLayout(
    const vkex::PipelineLayoutCreateInfo& create_info,
    vkex::PipelineLayout*                 p_object,
    const VkAllocationCallbacks*          p_allocator = nullptr
  );

  /** @fn DestroyPipelineLayout
   *
   */
  vkex::Result DestroyPipelineLayout(
    vkex::PipelineLayout          object,
    const VkAllocationCallbacks*  p_allocator = nullptr
  );

  /** @fn CreateQueryPool
   *
   */
  vkex::Result CreateQueryPool(
    const vkex::QueryPoolCreateInfo& create_info,
    vkex::QueryPool*                 p_object,
    const VkAllocationCallbacks*     p_allocator = nullptr
  );

  /** @fn DestroyQueryPool
   *
   */
  vkex::Result DestroyQueryPool(
    vkex::QueryPool              object,
    const VkAllocationCallbacks* p_allocator = nullptr
  );

  /** @fn CreateRenderPass
   *
   */
  vkex::Result CreateRenderPass(
    const vkex::RenderPassCreateInfo& create_info,
    vkex::RenderPass*                 p_object,
    const VkAllocationCallbacks*      p_allocator = nullptr
  );

  /** @fn DestroyRenderPass
   *
   */
  vkex::Result DestroyRenderPass(
    vkex::RenderPass              object,
    const VkAllocationCallbacks*  p_allocator = nullptr
  );

  /** @fn CreateRenderTargetView
   *
   */
  vkex::Result CreateRenderTargetView(
    const vkex::RenderTargetViewCreateInfo& create_info,
    vkex::RenderTargetView*                 p_object,
    const VkAllocationCallbacks*            p_allocator = nullptr
  );

  /** @fn DestroyRenderTargetView
   *
   */
  vkex::Result DestroyRenderTargetView(
    vkex::RenderTargetView        object,
    const VkAllocationCallbacks*  p_allocator = nullptr
  );

  /** @fn CreateSampler
   *
   */
  vkex::Result CreateSampler(
    const vkex::SamplerCreateInfo&  create_info,
    vkex::Sampler*                  p_object,
    const VkAllocationCallbacks*    p_allocator = nullptr
  );

  /** @fn DestroySampler
   *
   */
  vkex::Result DestroySampler(
    vkex::Sampler                 object,
    const VkAllocationCallbacks*  p_allocator = nullptr
  );

  /** @fn CreateSemaphore
   *
   */
  vkex::Result CreateSemaphore(
    const vkex::SemaphoreCreateInfo&  create_info,
    vkex::Semaphore*                  p_object,
    const VkAllocationCallbacks*      p_allocator = nullptr
  );

  /** @fn DestroySemaphore
   *
   */
  vkex::Result DestroySemaphore(
    vkex::Semaphore               object,
    const VkAllocationCallbacks*  p_allocator = nullptr
  );

  /** @fn CreateShaderModule
   *
   */
  vkex::Result CreateShaderModule(
    const vkex::ShaderModuleCreateInfo& create_info,
    vkex::ShaderModule*                 p_object,
    const VkAllocationCallbacks*        p_allocator = nullptr
  );

  /** @fn DestroyShaderModule
   *
   */
  vkex::Result DestroyShaderModule(
    vkex::ShaderModule            object,
    const VkAllocationCallbacks*  p_allocator = nullptr
  );

  /** @fn CreateShaderProgram
   *
   */
  vkex::Result CreateShaderProgram(
    const vkex::ShaderProgramCreateInfo&  create_info,
    vkex::ShaderProgram*                  p_object,
    const VkAllocationCallbacks*          p_allocator = nullptr
  );

  /** @fn DestroyShaderProgram
   *
   */
  vkex::Result DestroyShaderProgram(
    vkex::ShaderProgram           object,
    const VkAllocationCallbacks*  p_allocator = nullptr
  );

  /** @fn CreateStorageBuffer
   *
   */
  vkex::Result CreateStorageBuffer(
    const vkex::BufferCreateInfo&  create_info,
    vkex::Buffer*                  p_object,
    const VkAllocationCallbacks*   p_allocator = nullptr
  );

  /** @fn DestroyStorageBuffer
   *
   */
  vkex::Result DestroyStorageBuffer(
    vkex::Buffer                  object,
    const VkAllocationCallbacks*  p_allocator = nullptr
  );

  /** @fn CreateSwapchain
   *
   */
  vkex::Result CreateSwapchain(
    const vkex::SwapchainCreateInfo&  create_info,
    vkex::Swapchain*                  p_object,
    const VkAllocationCallbacks*      p_allocator = nullptr
  );

  /** @fn DestroySwapchain
   *
   */
  vkex::Result DestroySwapchain(
    vkex::Swapchain               object,
    const VkAllocationCallbacks*  p_allocator = nullptr
  );

  /** @fn CreateTexture
   *
   */
  vkex::Result CreateTexture(
    const vkex::TextureCreateInfo&  create_info,
    vkex::Texture*                  p_object,
    const VkAllocationCallbacks*    p_allocator = nullptr
  );

  /** @fn DestroyTexture
   *
   */
  vkex::Result DestroyTexture(
    vkex::Texture                 object,
    const VkAllocationCallbacks*  p_allocator = nullptr
  );

  /** @fn CreateVertexBuffer
   *
   */
  vkex::Result CreateVertexBuffer(
    const vkex::BufferCreateInfo& create_info,
    vkex::Buffer*                 p_object,
    const VkAllocationCallbacks*  p_allocator = nullptr
  );

  /** @fn DestroyVertexBuffer
   *
   */
  vkex::Result DestroyVertexBuffer(
    vkex::Buffer                  object,
    const VkAllocationCallbacks*  p_allocator = nullptr
  );

private:
  friend class CInstance;
  friend class IObjectStorageFunctions;

  /** @fn InitializeExtensions
   *
   */
  vkex::Result InitializeExtensions();

  /** @fn InitializeQueueRequests
   *
   */
  vkex::Result InitializeQueueRequests();

  /** @fn InitializeQueues
   *
   */
  vkex::Result InitializeQueues();

  /** @fn InternalCreate
   *
   */
  vkex::Result InternalCreate(
    const vkex::DeviceCreateInfo& create_info,
    const VkAllocationCallbacks*  p_allocator
  );

  /** @fn DestroyAllObjects
   *
   */
  vkex::Result DestroyAllStoredObjects(const VkAllocationCallbacks* p_allocator);

  /** @fn InternalDestroy
   *
   */
  vkex::Result InternalDestroy(const VkAllocationCallbacks* p_allocator);

  /** @fn SetInstance
   *
   */
  void SetInstance(vkex::Instance instance) {
    m_instance = instance;
  }

private:
  vkex::Instance                        m_instance = nullptr;
  DeviceCreateInfo                      m_create_info = {};
  std::vector<std::string>              m_found_extensions;
  std::vector<const char*>              m_c_str_extensions;
  std::vector<VkDeviceQueueCreateInfo>  m_vk_queue_create_infos;
  VkDeviceCreateInfo                    m_vk_create_info = {};
  VkDevice                              m_vk_object = VK_NULL_HANDLE;
  VmaAllocator                          m_vma_allocator = VK_NULL_HANDLE;

  std::vector<std::unique_ptr<CBuffer>>               m_stored_buffers;
  std::vector<std::unique_ptr<CCommandPool>>          m_stored_command_pools;
  std::vector<std::unique_ptr<CComputePipeline>>      m_stored_compute_pipelines;
  std::vector<std::unique_ptr<CDepthStencilView>>     m_stored_depth_stencil_views;
  std::vector<std::unique_ptr<CDescriptorPool>>       m_stored_descriptor_pools;
  std::vector<std::unique_ptr<CDescriptorSetLayout>>  m_stored_descriptor_set_layouts;
  std::vector<std::unique_ptr<CEvent>>                m_stored_events;
  std::vector<std::unique_ptr<CFence>>                m_stored_fences;
  std::vector<std::unique_ptr<CGraphicsPipeline>>     m_stored_graphics_pipelines;
  std::vector<std::unique_ptr<CImage>>                m_stored_images;
  std::vector<std::unique_ptr<CImageView>>            m_stored_image_views;
  std::vector<std::unique_ptr<CPipelineCache>>        m_stored_pipeline_caches;
  std::vector<std::unique_ptr<CPipelineLayout>>       m_stored_pipeline_layouts;
  std::vector<std::unique_ptr<CQueryPool>>            m_stored_query_pools;
  std::vector<std::unique_ptr<CQueue>>                m_stored_queues;
  std::vector<std::unique_ptr<CRenderPass>>           m_stored_render_passes;
  std::vector<std::unique_ptr<CRenderTargetView>>     m_stored_render_target_views;
  std::vector<std::unique_ptr<CSampler>>              m_stored_samplers;
  std::vector<std::unique_ptr<CSemaphore>>            m_stored_semaphores;
  std::vector<std::unique_ptr<CShaderModule>>         m_stored_shader_modules;
  std::vector<std::unique_ptr<CShaderProgram>>        m_stored_shader_programs;
  std::vector<std::unique_ptr<CSwapchain>>            m_stored_swapchains;
  std::vector<std::unique_ptr<CTexture>>              m_stored_textures;
};

} // namespace vkex

#endif // __VKEX_DEVICE_H__
// Copyright 2017 Chabloom LC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "_vulkan.h"

VKAPI_ATTR VkResult VKAPI_CALL vkAllocateCommandBuffers(
    VkDevice                           device,
    const VkCommandBufferAllocateInfo* pAllocateInfo,
    VkCommandBuffer*                   pCommandBuffers)
{
    for (uint32_t i = 0; i < pAllocateInfo->commandBufferCount; ++i)
    {
        HRESULT hr = device->Get()->CreateCommandList(device->GetPhysicalDevice()->GetIndex(), D3D12_COMMAND_LIST_TYPE_DIRECT, pAllocateInfo->commandPool->Get(), nullptr, IID_PPV_ARGS(&pCommandBuffers[i]->commandList));
        if (FAILED(hr))
        {
            return VkResultFromHRESULT(hr);
        }
        pCommandBuffers[i]->SetCommandAllocator(pAllocateInfo->commandPool->Get());
    }
    return VK_SUCCESS;
}

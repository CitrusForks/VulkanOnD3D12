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

VKAPI_ATTR VkResult VKAPI_CALL vkCreateImage(
    VkDevice                     device,
    const VkImageCreateInfo*     pCreateInfo,
    const VkAllocationCallbacks* pAllocator,
    VkImage*                     pImage)
{
    VkImage image = nullptr;
    if (pAllocator)
    {
        image = static_cast<VkImage>(pAllocator->pfnAllocation(nullptr, sizeof(VkImage_T), 8, VK_SYSTEM_ALLOCATION_SCOPE_DEVICE));
    }
    else
    {
        image = new VkImage_T();
    }

    D3D12_CLEAR_VALUE     clearValue    = {};
    D3D12_RESOURCE_STATES resourceState = D3D12_RESOURCE_STATE_COPY_DEST;
    D3D12_RESOURCE_FLAGS  resourceFlags = D3D12_RESOURCE_FLAG_NONE;
    if (pCreateInfo->usage & VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT)
    {
        clearValue = CD3DX12_CLEAR_VALUE(VkFormatToD3D12(pCreateInfo->format), 1.0f, 0);
        resourceFlags |= D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
        resourceFlags |= D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE;
    }

    D3D12_RESOURCE_DESC desc = {};
    switch (pCreateInfo->imageType)
    {
    case VK_IMAGE_TYPE_1D:
        desc = CD3DX12_RESOURCE_DESC::Tex1D(
            VkFormatToD3D12(pCreateInfo->format),
            pCreateInfo->extent.width,
            static_cast<UINT16>(pCreateInfo->arrayLayers),
            static_cast<UINT16>(pCreateInfo->mipLevels),
            resourceFlags);
        break;
    case VK_IMAGE_TYPE_2D:
        desc = CD3DX12_RESOURCE_DESC::Tex2D(
            VkFormatToD3D12(pCreateInfo->format),
            pCreateInfo->extent.width,
            pCreateInfo->extent.height,
            static_cast<UINT16>(pCreateInfo->arrayLayers),
            static_cast<UINT16>(pCreateInfo->mipLevels),
            pCreateInfo->samples,
            0,
            resourceFlags);
        break;
    case VK_IMAGE_TYPE_3D:
        desc = CD3DX12_RESOURCE_DESC::Tex3D(
            VkFormatToD3D12(pCreateInfo->format),
            pCreateInfo->extent.width,
            pCreateInfo->extent.height,
            static_cast<UINT16>(pCreateInfo->extent.depth),
            static_cast<UINT16>(pCreateInfo->mipLevels),
            resourceFlags);
        break;
    }

    D3D12_HEAP_PROPERTIES heapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

    HRESULT hr = device->Get()->CreateCommittedResource(
        &heapProperties,
        D3D12_HEAP_FLAG_NONE,
        &desc,
        resourceState,
        (resourceFlags & D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET) || (resourceFlags & D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL) ? &clearValue : nullptr,
        IID_PPV_ARGS(image->GetAddressOf()));
    if (FAILED(hr))
    {
        return VkResultFromHRESULT(hr);
    }

    *pImage = image;

    return VK_SUCCESS;
}

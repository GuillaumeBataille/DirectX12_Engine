#include "Precompiled_header.h"
#include "DataStruct.h"

D3D12_INPUT_ELEMENT_DESC vertexDesc[] = {
 {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
 D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
 {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12,
 D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
};

/* buffer's widdth = size * sizeof(typedata)*/
static inline CD3DX12_RESOURCE_DESC Buffer(
	UINT64 width,
	D3D12_RESOURCE_FLAGS flags = D3D12_RESOURCE_FLAG_NONE,
	UINT64 alignment = 0)
{
	return CD3DX12_RESOURCE_DESC(D3D12_RESOURCE_DIMENSION_BUFFER,
		alignment, width, 1, 1, 1,
		DXGI_FORMAT_UNKNOWN, 1, 0,
		D3D12_TEXTURE_LAYOUT_ROW_MAJOR, flags);
}

Vertex vertices[] =
{
 { XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(Colors::White) },
 { XMFLOAT3(-1.0f, +1.0f, -1.0f), XMFLOAT4(Colors::Black) },
 { XMFLOAT3(+1.0f, +1.0f, -1.0f), XMFLOAT4(Colors::Red) },
 { XMFLOAT3(+1.0f, -1.0f, -1.0f), XMFLOAT4(Colors::Green) },
 { XMFLOAT3(-1.0f, -1.0f, +1.0f), XMFLOAT4(Colors::Blue) },
 { XMFLOAT3(-1.0f, +1.0f, +1.0f), XMFLOAT4(Colors::Yellow) },
 { XMFLOAT3(+1.0f, +1.0f, +1.0f), XMFLOAT4(Colors::Cyan) },
 { XMFLOAT3(+1.0f, -1.0f, +1.0f), XMFLOAT4(Colors::Magenta) }
};

/*-------------------- CLASS AND FUNCTION TO MANAGE SOME OPERATIONS EASILY  --------------------*/

class d3dUtil
{
public:

    static bool IsKeyDown(int vkeyCode);                                                

    static std::string ToString(HRESULT hr);                                            // ErrorCodeToString

    static UINT CalcConstantBufferByteSize(UINT byteSize)                               // Get the size (multiple of 256) requiered for a buffer
    {
        return (byteSize + 255) & ~255;
    }

    static Microsoft::WRL::ComPtr<ID3DBlob> LoadBinary(const std::wstring& filename);   

    static Microsoft::WRL::ComPtr<ID3D12Resource> CreateDefaultBuffer(                  // Create a simple buffer with a source buffer
        ID3D12Device* device,
        ID3D12GraphicsCommandList* cmdList,
        const void* initData,
        UINT64 byteSize,
        Microsoft::WRL::ComPtr<ID3D12Resource>& uploadBuffer);

    static Microsoft::WRL::ComPtr<ID3DBlob> CompileShader(                              // Compile shaders with a filepath
        const std::wstring& filename,
        const D3D_SHADER_MACRO* defines,
        const std::string& entrypoint,
        const std::string& target);
};

extern const int gNumFrameResources;

/* Name setter to DX12 Objects for a better debugging */
inline void d3dSetDebugName(IDXGIObject* obj, const char* name)
{
    if (obj)
    {
        obj->SetPrivateData(WKPDID_D3DDebugObjectName, lstrlenA(name), name);
    }
}
inline void d3dSetDebugName(ID3D12Device* obj, const char* name)
{
    if (obj)
    {
        obj->SetPrivateData(WKPDID_D3DDebugObjectName, lstrlenA(name), name);
    }
}
inline void d3dSetDebugName(ID3D12DeviceChild* obj, const char* name)
{
    if (obj)
    {
        obj->SetPrivateData(WKPDID_D3DDebugObjectName, lstrlenA(name), name);
    }
}

/* Convert a std::string(ANSI) to a std::wstring(UNICODE) for API WIN mostly */
inline std::wstring AnsiToWString(const std::string& str)
{
    WCHAR buffer[512];
    MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, buffer, 512);
    return std::wstring(buffer);
}

/* Callback when errorcode is failed */
inline void ThrowIfFailed(HRESULT hr)
{
    if (FAILED(hr))
    {
        _com_error err(hr);
        std::wstring errMsg = L"Operation failed with HRESULT: " + std::to_wstring(hr) + L"\n" + err.ErrorMessage();
        throw std::runtime_error(std::string(errMsg.begin(), errMsg.end()));
    }
}

/* Free COM ptr */
inline void ReleaseCom(IUnknown* x)
{
    if (x)
    {
        x->Release();
        x = nullptr;
    }
}


/*-------------------- EXEPTION HANDER WHEN ERROR ARE GENERATED  --------------------*/

class DxException
{
public:
    DxException() = default;               
    DxException(
        HRESULT hr,
        const std::wstring& functionName,
        const std::wstring& filename,
        int lineNumber);

    std::wstring ToString()const;

    HRESULT ErrorCode = S_OK;       
    std::wstring FunctionName;
    std::wstring Filename;
    int LineNumber = -1;
};


/*----------------------- MESH & RENDERING STRUCTURE -----------------------*/

struct SubmeshGeometry
{
    UINT IndexCount = 0;
    UINT StartIndexLocation = 0;
    INT BaseVertexLocation = 0;

    DirectX::BoundingBox Bounds;
};

struct MeshGeometry
{
    std::string Name;

    /* CPU buffers - Blob = memory bloc genetic */
    Microsoft::WRL::ComPtr<ID3DBlob> VertexBufferCPU = nullptr; //Position - Normal - Texture - etc..
    Microsoft::WRL::ComPtr<ID3DBlob> IndexBufferCPU = nullptr; // Triangle ID

    /* GPU buffers - Resource = allocated memory like memorybloc, texture,copystruct, buffers, etc.. */
    Microsoft::WRL::ComPtr<ID3D12Resource> VertexBufferGPU = nullptr;
    Microsoft::WRL::ComPtr<ID3D12Resource> IndexBufferGPU = nullptr;

    /* Uploader Buffers - Temp buffer that will bring CPU data into GPU buffers */
    Microsoft::WRL::ComPtr<ID3D12Resource> VertexBufferUploader = nullptr;
    Microsoft::WRL::ComPtr<ID3D12Resource> IndexBufferUploader = nullptr;


    UINT VertexByteStride = 0;                      // To identify one vertex
    UINT VertexBufferByteSize = 0;                  // Total size in byte of the vertex buffer
    DXGI_FORMAT IndexFormat = DXGI_FORMAT_R16_UINT; // Triangle index format
    UINT IndexBufferByteSize = 0;                   // Total size in byte of the index buffer

    /* To handle many sub mesh with one MeshGeometry vertex/index buffer, use a SubmeshGeometry's map */
    std::unordered_map<std::string, SubmeshGeometry> DrawArgs;

    /* Get a view on the GPU vertex buffer  */
    D3D12_VERTEX_BUFFER_VIEW VertexBufferView()const
    {
        D3D12_VERTEX_BUFFER_VIEW vbv;
        vbv.BufferLocation = VertexBufferGPU->GetGPUVirtualAddress();
        vbv.StrideInBytes = VertexByteStride;
        vbv.SizeInBytes = VertexBufferByteSize;

        return vbv;
    }

    /* Get a view on the GPU index buffer  */
    D3D12_INDEX_BUFFER_VIEW IndexBufferView()const
    {
        D3D12_INDEX_BUFFER_VIEW ibv;
        ibv.BufferLocation = IndexBufferGPU->GetGPUVirtualAddress();
        ibv.Format = IndexFormat;
        ibv.SizeInBytes = IndexBufferByteSize;

        return ibv;
    }

    /* Free temporal Uploaders */
    void DisposeUploaders()
    {
        VertexBufferUploader = nullptr;
        IndexBufferUploader = nullptr;
    }
};

struct Light
{
    DirectX::XMFLOAT3 Strength = { 0.5f, 0.5f, 0.5f };
    float FalloffStart = 1.0f;                          // point/spot light only
    DirectX::XMFLOAT3 Direction = { 0.0f, -1.0f, 0.0f };// directional/spot light only
    float FalloffEnd = 10.0f;                           // point/spot light only
    DirectX::XMFLOAT3 Position = { 0.0f, 0.0f, 0.0f };  // point/spot light only
    float SpotPower = 64.0f;                            // spot light only
};

struct MaterialConstants
{
    DirectX::XMFLOAT4 DiffuseAlbedo = { 1.0f, 1.0f, 1.0f, 1.0f };
    DirectX::XMFLOAT3 FresnelR0 = { 0.01f, 0.01f, 0.01f };
    float Roughness = 0.25f;

    // Used in texture mapping.
    DirectX::XMFLOAT4X4 MatTransform = MathHelper::Identity4x4();
};

struct Material
{

    std::string Name;

    int MatCBIndex = -1;

    int DiffuseSrvHeapIndex = -1;

    int NormalSrvHeapIndex = -1;

    int NumFramesDirty = gNumFrameResources;

    DirectX::XMFLOAT4 DiffuseAlbedo = { 1.0f, 1.0f, 1.0f, 1.0f };
    DirectX::XMFLOAT3 FresnelR0 = { 0.01f, 0.01f, 0.01f };
    float Roughness = .25f;
    DirectX::XMFLOAT4X4 MatTransform = MathHelper::Identity4x4();
};

struct Texture
{
    std::string Name;

    std::wstring Filename;

    Microsoft::WRL::ComPtr<ID3D12Resource> Resource = nullptr;
    Microsoft::WRL::ComPtr<ID3D12Resource> UploadHeap = nullptr;
};
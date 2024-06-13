#pragma once

#include "Precompiled_header.h"
#include "3DappUtil.h"
#include "GameTimer.h"

class D3DApp
{
protected:

    D3DApp(HINSTANCE hInstance);
    D3DApp(const D3DApp& rhs) = delete;
    D3DApp& operator=(const D3DApp& rhs) = delete;
    virtual ~D3DApp();

public:
    
    /* Main functionnalities  */
    int Run();                  // Launch the gameloop
    virtual bool Initialize();  //Init the app
    virtual LRESULT MsgProc(    //MessageProcess
        HWND hwnd, 
        UINT msg, 
        WPARAM wParam, 
        LPARAM lParam);
    
    /* Getter */
    static D3DApp* GetApp();    // AppInstance static
    HINSTANCE AppInst()const;   // handleInstance that hold the app
    HWND MainWnd()const;        // handleWindow of the main window
    float AspectRatio()const;  
    bool Get4xMsaaState()const;
    void Set4xMsaaState(bool value);




protected:
    /* virtual method that will be overwitten */
    virtual void CreateRtvAndDsvDescriptorHeaps();            
    virtual void OnResize();    
    virtual void Update(const GameTimer& gt) = 0;
    virtual void Draw(const GameTimer& gt) = 0;

    virtual void OnMouseDown(WPARAM btnState, int x, int y) { }
    virtual void OnMouseUp(WPARAM btnState, int x, int y) { }
    virtual void OnMouseMove(WPARAM btnState, int x, int y) { }

protected:

    bool InitMainWindow();      // Setup a window
    bool InitDirect3D();        // Setup the Direct3D environment
    void CreateCommandObjects();// Create the command queue, command list and allocator
    void CreateSwapChain();     // Create the swapchain and the barrier/buffers

    void FlushCommandQueue();   // Clear the command queue

    ID3D12Resource* CurrentBackBuffer()const;                   // CurrentBackBuffer ressource (On GPU)
    D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView()const;   // CurrentBackBuffer view on the GPU
    D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView()const;        // Current DepthStencilView on the GPU



    /* Log informations & Stats */
    void LogAdapters();
    void LogAdapterOutputs(IDXGIAdapter* adapter);
    void LogOutputDisplayModes(IDXGIOutput* output, DXGI_FORMAT format);
    void CalculateFrameStats();

protected:

    static D3DApp* mApp;            //Unique static instance of the app
    HINSTANCE mhAppInst = nullptr;  // application instance handle
    
    /* Window parameters */
    HWND      mhMainWnd = nullptr;
    bool      mAppPaused = false;   
    bool      mMinimized = false;  
    bool      mMaximized = false;   
    bool      mResizing = false;   
    bool      mFullscreenState = false;

    // Set true to use 4X MSAA
    bool      m4xMsaaState = false;    // 4X MSAA enabled
    UINT      m4xMsaaQuality = 0;      // quality level (from 0 to maximum available on the hardware)


    GameTimer mTimer;

    Microsoft::WRL::ComPtr<IDXGIFactory4> mdxgiFactory;     // List of all graphic adaptator (hard or soft ware) like the GPU
    Microsoft::WRL::ComPtr<IDXGISwapChain> mSwapChain;      // The Swap chain
    Microsoft::WRL::ComPtr<ID3D12Device> md3dDevice;        // Device entity to bind to the GPU that will allow to use it with Direct3D

    Microsoft::WRL::ComPtr<ID3D12Fence> mFence;             // Synchronizer GPU/CPU 
    UINT64 mCurrentFence = 0;

    Microsoft::WRL::ComPtr<ID3D12CommandQueue> mCommandQueue;           // The stack that will store from CPU and send a list of instruction to GPU
    Microsoft::WRL::ComPtr<ID3D12CommandAllocator> mDirectCmdListAlloc; // The memory allocated to host command list in a command queue
    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> mCommandList;     // One or many instructions that will be sent to the command Queue to execute them in GPU

    static const int SwapChainBufferCount = 2;
    int mCurrBackBuffer = 0;
    Microsoft::WRL::ComPtr<ID3D12Resource> mSwapChainBuffer[SwapChainBufferCount];
    Microsoft::WRL::ComPtr<ID3D12Resource> mDepthStencilBuffer;

    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mRtvHeap;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mDsvHeap;

    D3D12_VIEWPORT mScreenViewport;
    D3D12_RECT mScissorRect;

    UINT mRtvDescriptorSize = 0;
    UINT mDsvDescriptorSize = 0;
    UINT mCbvSrvUavDescriptorSize = 0;

    // Derived class should set these in derived constructor to customize starting values.
    std::wstring mMainWndCaption = L"Probe Engine";
    D3D_DRIVER_TYPE md3dDriverType = D3D_DRIVER_TYPE_HARDWARE;
    DXGI_FORMAT mBackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
    DXGI_FORMAT mDepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
    int mClientWidth = base_width;
    int mClientHeight = base_height;
};
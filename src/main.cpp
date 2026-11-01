#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>
#include <array>
#include <cmath>
#include <string>
#include <vector>
#include <algorithm>
struct Entity { float x,y; int health; bool hostile; };
struct State {
 bool esp=true, xray=true, aura=false;
 int radius=140, range=90, tick=0, selected=-1;
 std::vector<Entity> entities{{140,110,90,true},{320,180,65,true},{510,130,100,false},{230,310,75,true}};
} state;
void Paint(HWND hwnd) {
 PAINTSTRUCT ps{}; HDC dc=BeginPaint(hwnd,&ps);
 RECT r{}; GetClientRect(hwnd,&r);
 HBRUSH bg=CreateSolidBrush(RGB(19,23,34)); FillRect(dc,&r,bg); DeleteObject(bg);
 SetBkMode(dc,TRANSPARENT); SetTextColor(dc,RGB(235,239,250));
 auto line=[&](int y,const wchar_t* t){TextOutW(dc,22,y,t,lstrlenW(t));};
 line(18,L"BEDROCK WORKBENCH | OFFLINE SIMULATION");
 line(52,L"F1 ESP    F2 X-Ray    F3 Kill Aura    +/- Range    ESC Exit");
 std::wstring status=L"ESP: "+std::wstring(state.esp?L"ON":L"OFF")+L"    X-Ray: "+(state.xray?L"ON":L"OFF")+L"    Aura: "+(state.aura?L"ON":L"OFF")+L"    Range: "+std::to_wstring(state.range);
 line(82,status.c_str());
 HBRUSH field=CreateSolidBrush(RGB(32,39,52)); RECT area{20,120,760,530};FillRect(dc,&area,field);DeleteObject(field);
 HPEN pen=CreatePen(PS_DOT,1,RGB(115,120,155));HGDIOBJ old=SelectObject(dc,pen);
 Ellipse(dc,370-state.range,320-state.range,370+state.range,320+state.range);
 SelectObject(dc,old);DeleteObject(pen);
 for(size_t i=0;i<state.entities.size();++i){
  const auto& e=state.entities[i]; int x=70+(int)e.x,y=145+(int)e.y;
  if(state.esp){HPEN p=CreatePen(PS_SOLID,2,e.hostile?RGB(255,100,115):RGB(90,210,160));auto o=SelectObject(dc,p);SelectObject(dc,GetStockObject(HOLLOW_BRUSH));Rectangle(dc,x-13,y-25,x+13,y+25);SelectObject(dc,o);DeleteObject(p);}
  if(state.xray){SetTextColor(dc,RGB(255,214,85));TextOutW(dc,x+18,y-8,L"ORE",3);}
  if(state.aura && state.selected==(int)i){SetTextColor(dc,RGB(255,90,90));TextOutW(dc,x-18,y-43,L"TARGET",6);}
 }
 SetTextColor(dc,RGB(180,193,214));line(550,L"Standalone simulator. No Minecraft process access or server bypass.");
 EndPaint(hwnd,&ps);
}
LRESULT CALLBACK Proc(HWND hwnd,UINT msg,WPARAM wp,LPARAM lp){
 switch(msg){
 case WM_KEYDOWN:
  if(wp==VK_ESCAPE){DestroyWindow(hwnd);return 0;}
  if(wp==VK_F1)state.esp=!state.esp;
  if(wp==VK_F2)state.xray=!state.xray;
  if(wp==VK_F3)state.aura=!state.aura;
  if(wp==VK_OEM_PLUS||wp==VK_ADD)state.range=std::min(200,state.range+10);
  if(wp==VK_OEM_MINUS||wp==VK_SUBTRACT)state.range=std::max(10,state.range-10);
  InvalidateRect(hwnd,nullptr,FALSE);return 0;
 case WM_TIMER:{
  state.tick++;state.selected=-1;
  if(state.aura){float best=(float)state.range*state.range;for(size_t i=0;i<state.entities.size();++i){auto&e=state.entities[i];if(!e.hostile)continue;float dx=e.x-300,dy=e.y-175;float d=dx*dx+dy*dy;if(d<best){best=d;state.selected=(int)i;}}}
  InvalidateRect(hwnd,nullptr,FALSE);return 0;}
 case WM_PAINT:Paint(hwnd);return 0;
 case WM_DESTROY:KillTimer(hwnd,1);PostQuitMessage(0);return 0;
 }return DefWindowProcW(hwnd,msg,wp,lp);
}
int WINAPI wWinMain(HINSTANCE inst,HINSTANCE,PWSTR,int show){
 WNDCLASSW wc{};wc.hInstance=inst;wc.lpfnWndProc=Proc;wc.lpszClassName=L"BedrockWorkbench";wc.hCursor=LoadCursorW(nullptr,IDC_ARROW);if(!RegisterClassW(&wc))return 1;
 HWND hwnd=CreateWindowExW(0,wc.lpszClassName,L"Bedrock Workbench",WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,CW_USEDEFAULT,810,650,nullptr,nullptr,inst,nullptr);if(!hwnd)return 2;
 ShowWindow(hwnd,show);SetTimer(hwnd,1,100,nullptr);MSG msg{};while(GetMessageW(&msg,nullptr,0,0)>0){TranslateMessage(&msg);DispatchMessageW(&msg);}return (int)msg.wParam;
}

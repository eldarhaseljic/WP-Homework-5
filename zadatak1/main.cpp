#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include <stdio.h>
#include <vector>

#define BLACKBUTTON 101
#define GREENBUTTON 102
#define YELLOWBUTTON 103
#define CIRCLEBUTTON 104
#define SQUAREBUTTON 105
#define RBUTTON 106
#define PENBUTTON 107
#define RESETBUTTON 108

#define BLACK 0
#define GREEN 1
#define YELLOW 2
#define CIRCLE 3
#define SQUARE 4
#define R 5
#define PEN 6
#define RESET 7

struct Object
{
    int boja,r,x,y,debljina,oblik;
};

static int PageSize = 50, PenSize = 2;
bool firstSize = true, firstSizePen = true;
COLORREF colors [3] = {RGB(0,0,0),RGB(0,255,0),RGB(255,255,0)};
std::vector<Object> objects;

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static int color = BLACK, shape = CIRCLE;
    PAINTSTRUCT ps;
    HDC hdc;
    POINT p;

    switch (message)                  /* handle the messages */
    {
    case WM_USER:
    {
        switch(wParam)
        {
        case BLACK:
        {
            color = BLACK;
            printf("The color is black\n");
            break;
        }
        case GREEN:
        {
            color = GREEN;
            printf("The color is green\n");
            break;
        }
        case YELLOW:
        {
            color = YELLOW;
            printf("The color is yellow\n");
            break;
        }
        case CIRCLE:
        {
            shape = CIRCLE;
            printf("The shape is circle\n");
            break;
        }
        case SQUARE:
        {
            shape = SQUARE;
            printf("The shape is square\n");
            break;
        }
        case RESET:
        {
            objects.clear();
            color = BLACK;
            shape = CIRCLE;
            InvalidateRect(hwnd,NULL,TRUE);
            printf("Invalidate rect\n");
            break;
        }
        }
        break;
    }
    case WM_RBUTTONDOWN:
    case WM_LBUTTONDOWN:
    {
        GetCursorPos(&p);
        ScreenToClient(hwnd,&p);
        hdc = GetDC(hwnd);

        if(PageSize<=0)
        {
            MessageBox(hwnd, "The size of R needs to be greather than 0","Dimensions error", MB_OK);
            ReleaseDC(hwnd,hdc);
            break;
        }

        if(PenSize<=0)
        {
            MessageBox(hwnd, "The size of pen needs to be greather than 0","Dimensions error", MB_OK);
            ReleaseDC(hwnd,hdc);
            break;
        }
        if(PenSize >= PageSize)
        {
            MessageBox(hwnd, "The size of pen can't be greather or equal the size of R","Dimensions error", MB_OK);
            ReleaseDC(hwnd,hdc);
            break;
        }

        SelectObject(hdc,CreatePen(PS_SOLID,PenSize,BLACK_PEN));
        SelectObject(hdc,CreateSolidBrush(colors[color]));

        if(shape == SQUARE)
        {
            Rectangle(hdc,p.x - PageSize/2,p.y - PageSize/2,p.x + PageSize/2,p.y + PageSize/2);
        }
        else if(shape == CIRCLE)
        {
            Ellipse(hdc,p.x - PageSize/2,p.y - PageSize/2,p.x + PageSize/2,p.y + PageSize/2);
        }
        Object temp;
        temp.boja = color;
        temp.oblik = shape;
        temp.debljina = PenSize;
        temp.r = PageSize;
        temp.x = p.x;
        temp.y = p.y;
        if( objects.empty() || (!((objects.at(objects.size()-1).boja == temp.boja) && (objects.at(objects.size()-1).debljina == temp.debljina) &&
                                  (objects.at(objects.size()-1).oblik == temp.oblik) && (objects.at(objects.size()-1).r == temp.r) &&
                                  (objects.at(objects.size()-1).x == temp.x) && (objects.at(objects.size()-1).y == temp.y))) )
        {
            objects.push_back(temp);
        }
        //printf("%d\n",objects.size());
        ReleaseDC(hwnd,hdc);
        break;
    }
    case WM_PAINT:
    {
        HDC hdc = BeginPaint(hwnd,&ps);
        for(auto i = 0; i<objects.size(); ++i)
        {
            SelectObject(hdc,CreatePen(PS_SOLID,objects.at(i).debljina,BLACK_PEN));
            SelectObject(hdc,CreateSolidBrush(colors[objects.at(i).boja]));

            if(objects.at(i).oblik == SQUARE)
            {
                Rectangle(hdc,objects.at(i).x - objects.at(i).r/2,
                          objects.at(i).y - objects.at(i).r/2,
                          objects.at(i).x + objects.at(i).r/2,
                          objects.at(i).y + objects.at(i).r/2);
            }
            else if(objects.at(i).oblik == CIRCLE)
            {
                Ellipse(hdc,objects.at(i).x - objects.at(i).r/2,
                        objects.at(i).y - objects.at(i).r/2,
                        objects.at(i).x + objects.at(i).r/2,
                        objects.at(i).y + objects.at(i).r/2);
            }

        }
        EndPaint(hwnd,&ps);
        break;
    }
    case WM_DESTROY:
        PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
        break;
    default:                      /* for messages that we don't deal with */
        return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}

void SetDefaultSettings (HWND hwnd)
{
    if(!SendMessage(GetDlgItem(hwnd,BLACKBUTTON),BM_GETCHECK,0,0))
    {
        if(SendMessage(GetDlgItem(hwnd,GREENBUTTON),BM_GETCHECK,0,0))
        {
            SendMessage(GetDlgItem(hwnd,GREENBUTTON),BM_SETCHECK,0,0);
        }
        else
        {
            SendMessage(GetDlgItem(hwnd,YELLOWBUTTON),BM_SETCHECK,0,0);
        }
        SendMessage(GetDlgItem(hwnd,BLACKBUTTON),BM_SETCHECK,1,0);
    }

    if(!SendMessage(GetDlgItem(hwnd,CIRCLEBUTTON),BM_GETCHECK,0,0))
    {
        SendMessage(GetDlgItem(hwnd,SQUAREBUTTON),BM_SETCHECK,0,0);
        SendMessage(GetDlgItem(hwnd,CIRCLEBUTTON),BM_SETCHECK,1,0);
    }
    SetWindowText(GetDlgItem(hwnd,RBUTTON),"50");
    SetWindowText(GetDlgItem(hwnd,PENBUTTON),"2");
}

LRESULT CALLBACK Settings (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static char StringValuePen [50];
    static char StringValueR [50];
    switch (message)                  /* handle the messages */
    {
    case WM_CREATE:
    {
        HINSTANCE hinst = (HINSTANCE) GetWindowLong(hwnd,GWL_HINSTANCE);

        CreateWindow("button","Colors",
                     WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
                     10,10,120,90, hwnd, NULL, hinst, NULL);
        CreateWindow( "button", "Black",
                      WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP,
                      25,35,90,15, hwnd, (HMENU)BLACKBUTTON, hinst, NULL);


        CreateWindow( "button", "Green",
                      WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
                      25,55,90,15, hwnd, (HMENU)GREENBUTTON, hinst, NULL);
        CreateWindow( "button", "Yellow",
                      WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
                      25,75,90,15, hwnd, (HMENU)YELLOWBUTTON, hinst, NULL);

        //Objekti
        CreateWindow ("button", "Shape",
                      WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
                      10,105,120,70, hwnd, NULL, hinst, NULL);
        CreateWindow("button", "Circle",
                     WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON| WS_GROUP,
                     25,130,90,15, hwnd,(HMENU)CIRCLEBUTTON, hinst,NULL);


        CreateWindow("button", "Square",
                     WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
                     25,150,90,15, hwnd,(HMENU)SQUAREBUTTON, hinst,NULL);

        //Precnik/Stranica
        CreateWindow ("button", "Size of page",
                      WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
                      10,180,120,55, hwnd, NULL, hinst, NULL);
        CreateWindow("edit","50",
                     WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER,
                     20,205,100,20, hwnd,(HMENU)RBUTTON, hinst, NULL);

        //Pen size
        CreateWindow ("button", "Size of pen",
                      WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
                      10,240,120,55, hwnd, NULL, hinst, NULL);
        CreateWindow("edit","2",
                     WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER,
                     20,265,100,20, hwnd,(HMENU)PENBUTTON, hinst, NULL);
        CreateWindow("button", "Reset",
                     WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                     10,300,120,35, hwnd,(HMENU)RESETBUTTON, hinst, NULL);

        SendMessage(GetDlgItem(hwnd,BLACKBUTTON), BM_SETCHECK, 1,0);
        SendMessage(GetDlgItem(hwnd,CIRCLEBUTTON), BM_SETCHECK, 1,0);
        break;
    }
    case WM_COMMAND:
    {
        switch(LOWORD(wParam))
        {
        case BLACKBUTTON:
        {
            HWND main = GetWindow(hwnd,GW_OWNER);
            SendMessage(main,WM_USER, BLACK, 0);
            break;
        }
        case GREENBUTTON:
        {
            HWND main = GetWindow(hwnd,GW_OWNER);
            SendMessage(main,WM_USER, GREEN, 0);
            break;
        }
        case YELLOWBUTTON:
        {
            HWND main = GetWindow(hwnd,GW_OWNER);
            SendMessage(main,WM_USER, YELLOW, 0);
            break;
        }
        case CIRCLEBUTTON:
        {
            HWND main = GetWindow(hwnd,GW_OWNER);
            SendMessage(main,WM_USER, CIRCLE, 0);
            break;
        }
        case SQUAREBUTTON:
        {
            HWND main = GetWindow(hwnd,GW_OWNER);
            SendMessage(main,WM_USER, SQUARE, 0);
            break;
        }
        case RBUTTON:
        {

            GetWindowText(GetDlgItem(hwnd,RBUTTON), StringValueR, sizeof(StringValueR));
            PageSize = atoi(StringValueR);

            if(firstSize)
            {
                if(PageSize == 0)
                {
                    PageSize=50;
                }
                else
                {
                    firstSize = false;
                    PageSize=50;
                }
            }
            break;
        }
        case PENBUTTON:
        {

            GetWindowText(GetDlgItem(hwnd,PENBUTTON), StringValuePen, sizeof(StringValuePen));
            PenSize = atoi(StringValuePen);

            if(firstSizePen)
            {
                if(PenSize == 0)
                {
                    PenSize=2;
                }
                else
                {
                    firstSizePen = false;
                    PenSize=2;
                }
            }
            break;
        }
        case RESETBUTTON:
        {
            HWND main = GetWindow(hwnd,GW_OWNER);
            SendMessage(main,WM_USER, RESET, 0);
            SetDefaultSettings(hwnd);
            break;
        }
        }
        break;
    }
    case WM_DESTROY:
        PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
        break;
    default:                      /* for messages that we don't deal with */
        return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}



/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");

int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */

    //Hwnd posjedovanog prozora
    HWND hwndSecond;

    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = "Roditelj";
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    //Pojsedovani prozor inicijalizacija klase
    wincl.lpszClassName = "Posjedovani prozor";
    wincl.lpfnWndProc = Settings;

    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    int temp=40;
    SIZE sz;
    sz.cx = 544;
    sz.cy = 375;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
               0,                   /* Extended possibilites for variation */
               "Roditelj",          /* Classname */
               _T("Main screen"),       /* Title Text */
               WS_OVERLAPPEDWINDOW, /* default window */
               temp,       /* Windows decides the position */
               temp,       /* where the window ends up on the screen */
               sz.cx,                 /* The programs width */
               sz.cy,                 /* and height in pixels */
               HWND_DESKTOP,        /* The window is a child-window to desktop */
               NULL,                /* No menu */
               hThisInstance,       /* Program Instance handler */
               NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);


    /* The class is registered, let's create the program*/
    hwndSecond = CreateWindowEx (
                     0,                   /* Extended possibilites for variation */
                     "Posjedovani prozor",          /* Classname */
                     _T("Settings"),       /* Title Text */
                     WS_ACTIVECAPTION,  /* default window */
                     temp+sz.cx,       /* Windows decides the position */
                     temp,       /* where the window ends up on the screen */
                     150,                 /* The programs width */
                     sz.cy,                 /* and height in pixels */
                     hwnd,        /* The window is a child-window to desktop */
                     NULL,                /* No menu */
                     (HINSTANCE) GetWindowLong(hwnd,GWLP_HINSTANCE),       /* Program Instance handler */
                     NULL                 /* No Window Creation data */
                 );

    /* Make the window visible on the screen */
    ShowWindow (hwndSecond, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}

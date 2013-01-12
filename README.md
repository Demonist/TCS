TCS - Tickets Control System

Система контроля билетов.

===

Coding style:
  Кодировка исходного кода только UTF-8.
  Для табуляции используется только TAB, никаких пробелов!
  Никаких SELECT*.
  При git pull желателен параметр --rebase, иначе замучаемся со слияниями. (Qt->Инструменты->Параметры->Контроль версий->Git->Установить галочку "Принимать с преобразованием")
  git push используется с параметром --force только для amned коммитов и только если ВАШ коммит последний(!).
  Использование своеобразной венгерской нотации в именовании элементов интерфейса:
  
    hsp - Horizontal Spacer
    vsp - Vertial Spacer
    
    pbn - Push Button
    tbn - Tool Botton
    rbn - Radio Button
    chbx - Check Box
    clbn - Command Link Button
    bbx - Button Box
    
    lv - List View
    tv - Tree View
    tbv - Table View
    cv - Column View
    
    lw - List Widget
    tw - Tree Widget
    tbw - Table Widget
    
    gbx - Group Box
    sa - Scroll Area
    tbx - Tool Box
    tabw - Tab Widget
    sw - Stacked Widget
    f - Frame
    w - Widget
    mdi - MdiArea
    dw - Dock Widget
    
    cbx - Combo Box
    fcbx - Font Combo Box
    le - LineEdit
    te - Text Edit
    pte - Plain Text Edit
    sbx - Spin Box
    dsbx - Double Spin Box
    time - Time Edit
    de - Date Edit
    dte - DateTime Edit
    d - Dial
    hsb - Horizontal Scroll Bar
    vsb - Vertical Scroll Bar
    hs - Horizontal Slider
    vs - Vertical Slider
    
    l - Label
    tbr - Text Browser
    gv - Graphics View
    c - Calendar
    lcd - LCD Number
    pb - Progress Bar
    hl - Horizontal Line
    vl - Vertical Line
    dv - QDeclarativeView
    wv - QWebView
    
    
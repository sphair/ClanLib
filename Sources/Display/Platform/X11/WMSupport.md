# X11 Window Manager Test Plan and Support Log

## Rationale

To make sure ClanLib's X11 implementation works on major standards-compliant
windowing managers at all times.


## Window Managers / Platforms Supported

Use the Git blame tool to see when a particular WM was recorded here.

- KDE 4.14.11
  Status: Everything just works.

- Xfce 4.12 (running Xfwm 4.12.3)
  Status: Everything works, but `_NET_REQUEST_FRAME_EXTENTS` is malfunctioning
          despite reportedly being fixed recently.


## Test plan

If something doesn't work right on your end, please go through the following
test plan and report the problem numbers (i.e. ***1c***), your WM and any
additional behaviour you find odd to us. It will help us track down the
problem and fix it.

Note that this test plan is only intended to test X11 window related problems.
There could be bugs in the UI module that causes some of these problems and
those problems are not tested or covered here.

1. Run UI::HelloWorld Example.

2. Move pointer over the button element and see if it responds to being hovered
   over.

    a. If a crash occurs as pointer is moving onto the window:
       **Failure to process X11 events**

    b. If a crash occurs as pointer is moving onto the button:
       **Failure to create tooltip window**

    c. If button look changes to a hovered look even though the pointer is
       located **outside** of the button:
       **X11 pointer motion event offset**

       > Please check if offset is very similar to the lengths of the top-side
       > and left-side decorator frames of the window. This would mean it is
       > related to how the windowing system is offsetting the client area.

    d. If the button (or other UI elements) does not change looks when hovered
       over:
       **X11 event polling failure** or **Window redraw failure**

    e. If a tooltip does not appear when mouse is moved on to the button:
       ***Known unfixed bug***

3. Move and resize the window.

    a. If contents of the window do not not change, despite the window now being
       smaller or larger:
       **Window redraw failure** or **Window resize failure**

    b. If a crash occurs while or right after moving and/or resizing the window:
       **X11 event polling failure** or **Window resize failure**

    b. If the window moves or resizes unnaturally (not following the movement of
       the pointer):
       **X11 pointer motion event offset** or **Window reposition offset error**

    c. Perform Step 2 again and check if problem "1c", "1d" and/or "1e" occurs.

3. Click on the button on the main window.

    a. If a crash occurs after button is pressed:
       **Failure to create modal dialog**

    b. If the modal dialog did not appear after button is pressed and the main
       window no longer responds:
       **Failure when creating modal dialog**

    c. If the contents of modal dialog is a black background:
       ***Known unfixed bug***. Please test if moving and/or resizing the dialog
       causes the contents to be drawn.

    d. Perform Step 3 on the dialog box and test if any of the problems occur.

4. Click on the button on the dialog box. The dialog box should close.

    a. If a crash occurs after pressing the button:
       **Failure to unmap/delete dialog box**

    b. If the button does not respond:
       **X11 input event error.**

5. Click on the button on the main window to create the dialog box again. Now,
   close the window through your Window Manager (i.e. hit Alt+F4, click on Close
   button on window frame, etc.).

    a. If the window does not close:
       **Failure to respond to window delete event.**

    b. If a crash occurs when closing the dialog box:
       **Failure to unmap/delete dialog box.**

6. Test clipboard copy/paste support.

   On the text-area, copy text with [Ctrl]+[C] and paste text with [Ctrl]+[V].
   Only text is supported at the moment.

7. Close the main window.

    a. If the window does not close:
       **Failure to respond to window delete event.**

    b. If a crash occurs when closing the main window:
       **Failure to unmap/delete main window.**


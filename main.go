package main

import (
  "os"
)

func main() {
  // TODO: Realize cli also
  if len(os.Getenv("DISPLAY")) != 0 {
    gui_start()
  } else {
    tui_start()
  }
}
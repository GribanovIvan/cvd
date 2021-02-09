package main

import (
  "fyne.io/fyne/v2/app"
  "fyne.io/fyne/v2/container"
  "fyne.io/fyne/v2/widget"
)

func gui_start() {
  // TODO: Add more interaction
  app := app.New()
  window := app.NewWindow("CVD")

  mean := widget.NewLabel("CVD - Cool Video Downloader")
  input := widget.NewEntry()

  input.SetPlaceHolder("Введите релиз")

  window.SetContent(container.NewVBox(
    mean, input,
  ))

  window.ShowAndRun()
}
# Maybe, someone other will adopt that shit for Windows

OUT=cvd

if [[ -z `which go` ]]; then
  echo "Damn, install golang"
  exit 1
elif [[ -z `which goupx` ]]; then
  echo "Install goupx util first"
  echo "Don't forget to install upx also"
  exit 1
fi

go build -o $OUT -ldflags="-s -w" -p 6 -trimpath .
if [[ $? != 0 ]]; then
  exit 1
fi

goupx $OUT
if [[ $? != 0 ]]; then
  exit 1
fi

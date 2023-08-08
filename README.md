# DeKafkaAE

A port of the DeKafka avisynth function, but as an Adobe After Effects plug-in.

## References

[Original source code](http://avisynth.nl/index.php/DeKafka)

```
function dekafka(clip clip, int Xstart, int Ystart, int X, int Y, int Amount)
{
  ytop = Ystart
  ybot = Ystart + Y
  xleft = Xstart
  xright = Xstart + X
  topline = clip.Crop(Xstart, ytop-2, X, 2)
  bottomline = clip.Crop(Xstart, ybot, X, 2)
  leftline = clip.Crop(xleft-2, ytop, 2, Y)
  rightline = clip.Crop(xright, ytop, 2, Y)
  logosrc_hor = StackVertical(topline, bottomline).Blur(0, 1.58).BilinearResize(X, Y)
  logosrc_ver = StackHorizontal(leftline, rightline).Blur(1.58, 0).BilinearResize(X, Y)
  Amount2 = (Y>=2*X) ? 255 : 128*Y/X
  # Amount2 is small if X >> Y => logoscr_hor is dominant
  logosrc = Layer(logosrc_hor, logosrc_ver, "add", Amount2)
  clip = clip.Layer(logosrc, "add", Amount, Xstart, Ystart)
  return clip
}
```

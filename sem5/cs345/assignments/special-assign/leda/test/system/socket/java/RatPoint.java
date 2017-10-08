import java.awt.Point;

class RatPoint {
  int x = 0;
  int y = 0;
  int z = 1;

  public RatPoint() {}
  public RatPoint(int xc, int yc) {
    x = xc;
    y = yc;
  }

  public RatPoint(int xc, int yc, int zc) {
    x = xc;
    y = yc;
    z = (zc == 0) ? 1 : zc;
  }

  public Point toPoint() { return new Point(x/z, y/z); }
  public static RatPoint pointToRatPoint(Point p) { return new RatPoint(p.x, p.y); }
}

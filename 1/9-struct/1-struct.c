struct foo {
  short a;
  int b;
  float c;
};

typedef struct {
  short d;
  int e;
  float f;
} bar;

typedef struct _baz {
  short a;
  int b;
  float c;
} baz;

int main (int argc, char const *argv[])
{
  foo a;
  bar b;
  baz c;

  return 0;
}

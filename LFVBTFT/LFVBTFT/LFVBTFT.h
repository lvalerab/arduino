class utft {
  public:
    byte RD;
    byte WR;
    byte RS;
    byte CS;
    byte REST;
    void utft(byte pRD, byte pWR, byte pRS, byte pCS, byte pREST);
    void Writ_Bus(char VH);
    void Write_Com(char VH);
    void Write_Data(char VH);
    void Write_Com_Data(int com, int dat);
    void Address_set(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);
    void InitDPin(void)
    void InitAPin(void)
    void Init();
    void Clear(unsigned int c);
    void Punto(unsigned int x, unsigned int y, unsigned int c);
    void HLine(unsigned int x, unsigned int y, unsigned int l, unsigned int c);
    void VLine(unsigned int x, unsigned int y, unsigned int l, unsigned int c);
    void Rectangulo(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c);
    void Rectangulof(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c);
}

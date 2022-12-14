class Keyboard{
    public:
        Keyboard();
        char getChar();
        void loop();
        void quit();
    private:
        char keyQueue[0x400];
        bool isQuit = false;
        int outputIdx = 0;
        int inputIdx = 0;
};
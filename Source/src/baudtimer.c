// PSK31 decoding from https://github.com/zenmetsu/radioModem adapted for the sm32f4 processor
// credit Jason Westervelt (zenmetsu) vespira@gmail.com

//Timer control routines
void baud_timer_init()
{
  #ifdef ___MAPLE
    timer4.pause();                                    // Pause the timer while configuring it
    timer4.setMode(TIMER_CH1, TIMER_OUTPUT_COMPARE);   // Set up interrupt on channel 1
    timer4.setCount(0);                                // Reset count to zero

    timer4.setPrescaleFactor(72);                      // Timer counts at 72MHz/72 = 1MHz  1 count = 1uS
    timer4.setOverflow(0xFFFF);                        // reset occurs at 15.259Hz
    timer4.refresh();                                  // Refresh the timer's count, prescale, and overflow
    timer4.resume();                                   // Start the timer counting
  #endif

  #ifdef ___ARDUINO
    cli();                               // stop interrupts during configuration
    TCCR1A = 0;                          // Clear TCCR1A register
    TCCR1B = 0;                          // Clear TCCR1B register
    TCNT1  = 0;                          // Initialize counter value
    OCR1A  = 0xFFFF;                     // Set compare match register to maximum value
    TCCR1B |= (1 << WGM12);              // CTC mode
                                         // We want 1uS ticks, for 16MHz CPU, we use prescaler of 16
                                         // as 1MHz = 1uS period, but Arduino is lame and only has
                                         // 3 bit multiplier, we can have 8 (overflows too quickly)
                                         // or 64, which operates at 1/4 the desired resolution
    TCCR1B |= (1 << CS11);               // Configure for 8 prescaler
    TIMSK1 |= (1 << OCIE1A);             // enable compare interrupt
    sei();                               // re-enable interrupts
  #endif

  #ifdef ___TEENSY
    FTM0_MODE |= FTM_MODE_WPDIS;

    FTM0_CNT = 0;
    FTM0_CNTIN = 0;
    FTM0_SC |= FTM_SC_PS(7);
    FTM0_SC |= FTM_SC_CLKS(1);
    FTM0_MOD = 0xFFFF;
    FTM0_MODE |= FTM_MODE_FTMEN;
    /*
    PIT_LDVAL1 = 0x500000;
    PIT_TCTRL1 = TIE;
    PIT_TCTRL1 |= TEN;
    PIT_TFLG1 |= 1;
    */
  #endif
}



void baud_timer_restart()
{
  #ifdef ___MAPLE
    timer4.setCount(0);
    timer4.refresh();
  #endif

  #ifdef ___ARDUINO
    TCNT1 = 0;
    baudOverflow = false;
  #endif

  #ifdef ___TEENSY
    FTM0_CNT = 0x0;
  #endif
}



unsigned int baud_time_get()
{
  // FIXME: not implemented yet
  unsigned int tmp = 0;
  return tmp;
}

#ifdef ___ARDUINO
  ISR(TIMER1_COMPA_vect)
  {
    baudOverflow = true;                   // ISR called on overflow, set overflow flag
  }
#endif

#ifdef ___TEENSY
  void ftm0_isr(void)
  {
  }
#endif


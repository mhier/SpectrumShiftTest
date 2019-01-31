#!/bin/bash -e

# compile
cd ~/doocs/library/common/serverlib
make clean || exit 125
make localinstall -j5 || exit 125
cd ~/doocs/library/common/DOOCSdzmqlib
make clean || exit 125
make localinstall -j5 || exit 125
cd ~/ChimeraTK/build/doocsllrfwrapper-Debug
make clean || exit 125
make install -j5 || exit 125
cd ~/ControlSystemServers/build/llrfctrlsrf
make clean || exit 125
make -j5 || exit 125

# launch server in background
( ./llrfctrlsrf ) &

# wait until server is up
while ! doocsget -c TEST.DOOCS/LOCALHOST_610498009/VS.TEST/AMPL.SAMPLE ; do sleep 5 ; done
sleep 5

# launch test in background
cd ~/ControlTools/build/SpectrumShiftTest-Debug
rm -f TEST_FAILED
( ./SpectrumShiftTest || touch TEST_FAILED ) &

# wait for failure
for (( i=0; i<60; i++ )); do
  echo "==== Iteration ${i}..."
  sleep 1
  if [ -e TEST_FAILED ]; then
    killall -9 llrfctrlsrf || true
    killall -9 SpectrumShiftTest || true
    sleep 1
    echo "==== FAILED!"
    exit 1
  fi
done

killall -9 llrfctrlsrf || true
killall -9 SpectrumShiftTest || true
sleep 1
echo "==== PASSED!"

exit 0


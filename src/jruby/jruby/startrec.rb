require "ruboto.rb"
java_import "android.media.MediaRecorder" 
$r=MediaRecorder.new 
$r.setAudioSource(MediaRecorder::AudioSource::MIC);
$r.setOutputFormat(MediaRecorder::OutputFormat::THREE_GPP);
$r.setAudioEncoder(MediaRecorder::AudioEncoder::AMR_NB);
$r.setOutputFile('/mnt/sdcard/rec.3gpp');
$r.prepare
$r.start   # Recording is now started
 
 # recorder.stop();
 # recorder.reset();   // You can reuse the object by going back to setAudioSource() step
 # recorder.release();
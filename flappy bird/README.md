Flappy Bird
===========
1. win32:working Directory :$(ProjectDir)..\Resources
2. 重新指定引用cocos2d路径
3. android手机音量建响应 修改cocos2d\cocos\2d\platform\android\java\src\org\cocos2dx\lib\Cocos2dxGLSurfaceView.java文件onKeyDown函数，最终如下:

```
public boolean onKeyDown(final int pKeyCode, final KeyEvent pKeyEvent) {
   switch (pKeyCode) {
	case KeyEvent.KEYCODE_BACK:
	case KeyEvent.KEYCODE_MENU:
		this.queueEvent(new Runnable() {
			public void run() {
			 Cocos2dxGLSurfaceView.this.mCocos2dxRenderer.handleKeyDown(pKeyCode);
			}
		});
		return true;
	case KeyEvent.KEYCODE_VOLUME_UP:{
	     AudioManager audio = (AudioManager) this.getContext().getSystemService(Context.AUDIO_SERVICE);
	     audio.adjustStreamVolume(AudioManager.STREAM_MUSIC, AudioManager.ADJUST_RAISE, 					AudioManager.FLAG_PLAY_SOUND | AudioManager.FLAG_SHOW_UI);
	     }
	     return true;
	case KeyEvent.KEYCODE_VOLUME_DOWN:{
AudioManager audio = (AudioManager) this.getContext().getSystemService(Context.AUDIO_SERVICE);
audio.adjustStreamVolume(AudioManager.STREAM_MUSIC, AudioManager.ADJUST_LOWER, AudioManager.FLAG_PLAY_SOUND | AudioManager.FLAG_SHOW_UI);
	     }
	     return true;
	default:
	      return super.onKeyDown(pKeyCode, pKeyEvent);
	}
}
```

修改proj.android\AndroidManifest.xml设置

```
<uses-permission android:name="android.permission.MODIFY_AUDIO_SETTINGS" /> 
```

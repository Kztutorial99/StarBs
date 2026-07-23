# ===============================
# ProGuard rules for AIDE project
# ===============================

# Keep your main activity
-keep class com.android.support.MainActivity { *; }

# Keep all classes that extend essential Android components
-keep class * extends android.app.Activity { *; }
-keep class * extends android.app.Application { *; }
-keep class * extends android.app.Service { *; }
-keep class * extends android.content.BroadcastReceiver { *; }
-keep class * extends android.content.ContentProvider { *; }

# Keep constructors used in XML (layouts, views)
-keepclassmembers class * {
    public <init>(android.content.Context, android.util.AttributeSet);
}

# Keep native methods (NDK)
-keepclasseswithmembernames class * {
    native <methods>;
}

# Keep enums
-keepclassmembers enum * { *; }

# Keep annotations (optional)
-keepattributes *Annotation*

# Obfuscate everything else (default)
# No need to keep other class/method names

package com.xiaying.gpapi;

import com.xiaying.renascence.*;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by jiangxiaotang on 16/5/19.
 */
public class Renascence extends AutoRecycle{
    static {
        System.load("/Users/jiangxiaotang/Documents/Renascence/java/libGPJni.so");
    }

    public class Content extends AutoRecycle{
        private SWIGTYPE_p_GPContents mNativeContent;
        protected Content(SWIGTYPE_p_GPContents contents) {
            mNativeContent = contents;
        }

        public Content(double value) {
            mNativeContent = RenascenceBasic.GP_Contents_CreateDouble(value);
        }

        public Content(String value) {
            mNativeContent = RenascenceBasic.GP_Contents_CreateString(value);
        }

        public int size() {
            return RenascenceBasic.GP_Contents_Size(mNativeContent);
        }

        public void save(String outputPath, int n) {
            SWIGTYPE_p_GPWStream swigtype_p_gpStream = RenascenceBasic.GP_WStream_Create(outputPath);
            RenascenceBasic.GP_Contents_Save(mNativeContent, swigtype_p_gpStream, n);
            RenascenceBasic.GP_WStream_Destroy(swigtype_p_gpStream);
        }

        @Override
        public void recycle() {
            if (null != mNativeContent) {
                RenascenceBasic.GP_Contents_Destroy(mNativeContent);
                mNativeContent = null;
            }
        }

        protected SWIGTYPE_p_GPContents get() {
            return mNativeContent;
        }
    }


    public class AutoFunction extends AutoRecycle{
        private SWIGTYPE_p_IGPAutoDefFunction mNative;
        protected AutoFunction(String formula, String inputType) {
            mNative = RenascenceBasic.GP_Function_Create_ByFormula(mProducer, formula, inputType, null);
        }

        public void recycle() {
            if (null != mNative) {
                RenascenceBasic.GP_Function_Destroy(mNative);
                mNative = null;
            }
        }

        public Content run(Content input) {
            SWIGTYPE_p_GPContents output = RenascenceBasic.GP_Function_Run(mNative, input.get());
            return new Content(output);
        }
    }


    private SWIGTYPE_p_AGPProducer mProducer = null;
    public Renascence(String[] metaDataPaths) {
        SWIGTYPE_p_p_GPStream swigtypePPGpStreams = RenascenceBasic.GP_Streams_Malloc(metaDataPaths.length);
        ArrayList<SWIGTYPE_p_GPStream> forReleaseStreams = new ArrayList<SWIGTYPE_p_GPStream>();
        for (int i=0; i<metaDataPaths.length; ++i) {
            SWIGTYPE_p_GPStream swigtype_p_gpStream = RenascenceBasic.GP_Stream_Create(metaDataPaths[i]);
            forReleaseStreams.add(swigtype_p_gpStream);
            RenascenceBasic.GP_Streams_Set(swigtypePPGpStreams, swigtype_p_gpStream, i);
        }
        mProducer = RenascenceBasic.GP_Producer_Create(swigtypePPGpStreams, null, metaDataPaths.length, 2);//GPStreamADF
        for (SWIGTYPE_p_GPStream swigtype_p_gpStream : forReleaseStreams) {
            RenascenceBasic.GP_Stream_Destroy(swigtype_p_gpStream);
        }
        RenascenceBasic.GP_Streams_Free(swigtypePPGpStreams);
    }

    @Override
    public void recycle() {
        if (null != mProducer) {
            RenascenceBasic.GP_Producer_Destroy(mProducer);
            mProducer = null;
        }
    }

    public AutoFunction createFunction(String formula, String inputType) {
        return new AutoFunction(formula, inputType);
    }

    public Content createContent(String[] dataPath, String typeNames) {
        SWIGTYPE_p_p_GPStream swigtypePPGpStreams = RenascenceBasic.GP_Streams_Malloc(dataPath.length);
        ArrayList<SWIGTYPE_p_GPStream> forReleaseStreams = new ArrayList<SWIGTYPE_p_GPStream>();
        for (int i=0; i<dataPath.length; ++i) {
            SWIGTYPE_p_GPStream swigtype_p_gpStream = RenascenceBasic.GP_Stream_Create(dataPath[i]);
            forReleaseStreams.add(swigtype_p_gpStream);
            RenascenceBasic.GP_Streams_Set(swigtypePPGpStreams, swigtype_p_gpStream, i);
        }
        SWIGTYPE_p_GPContents contents = RenascenceBasic.GP_Contents_Load(mProducer, swigtypePPGpStreams, typeNames, dataPath.length);
        for (SWIGTYPE_p_GPStream swigtype_p_gpStream : forReleaseStreams) {
            RenascenceBasic.GP_Stream_Destroy(swigtype_p_gpStream);
        }
        RenascenceBasic.GP_Streams_Free(swigtypePPGpStreams);
        return new Content(contents);
    }

    public Content merge(List<Content> contents) {
        Content output = new Content(RenascenceBasic.GP_Contents_CreateCollector());
        for (Content c : contents) {
            int size = c.size();
            for (int i=0; i<size; ++i) {
                RenascenceBasic.GP_Contents_Collect(output.get(), c.get(), i);
            }
        }
        return output;	
    }

    public static void setStreamBasicPath(String path) {
        RenascenceBasic.GP_Set_Stream_Path(path);
    }
    public static void setLibBasicPath(String path) {
        RenascenceBasic.GP_Set_Lib_Path(path);
    }

}

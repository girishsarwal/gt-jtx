package com.gluedtomatoes.artrix;

import java.util.concurrent.LinkedBlockingQueue;

/**
 * Created by gsarwal on 5/8/2015.
 */
public class RenderQueue {
    private LinkedBlockingQueue<Renderable> renderCandidates;
    public void Process(){
        Renderable r;
        while((r = renderCandidates.poll()) != null){
            r.render();
        }
    }

    RenderQueue(){
        renderCandidates = new LinkedBlockingQueue<>();
    }

    public void Enque(Renderable renderable){
        renderCandidates.add(renderable);
    }
    public void Remove(Renderable renderable){
        renderCandidates.remove(renderable);
    }
    public void Purge(){
        renderCandidates.clear();
    }
}

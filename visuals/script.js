(function(){
    
    if ( ! Detector.webgl ) {
        
        Detector.addGetWebGLMessage();
        document.getElementById( 'container' ).innerHTML = "";
        
    }

    window.AudioContext = window.AudioContext || window.webkitAudioContext;

    var audioContext = new AudioContext(),
        audioInput = null,
        realAudioInput = null,
        inputPoint = null,
        audioRecorder = null,

        storeUniformsLocally = function() {
            localStorage.uniforms = JSON.stringify({
                curveScaleVal:uniforms.curvatureScale.value,
                curveValX:uniforms.curvature.value.x,
                curveValY:uniforms.curvature.value.y,
            });
        },

        retrieveUniformsLocally = function() {
            if(typeof localStorage.uniforms === 'undefined'){
                console.log('No locally stored uniforms');
                return;
            }
            
            var _uniforms = JSON.parse(localStorage.uniforms);
            
            uniforms.curvatureScale.value = _uniforms.curveScaleVal;
            uniforms.curvature.value.x = _uniforms.curveValX;
            uniforms.curvature.value.y = _uniforms.curveValY;
            
        },

        uniforms =  {
            curvature : { type: "v2", value: new THREE.Vector2(1,0.5625) },
            curvatureScale : { type: "f", value: 50 }
        },
        // update uniforms from localStorage
    // fullscreen toggle
        video      = document.createElement('video'),
        videoTexture,
        hasUserMedia = navigator.webkitGetUserMedia ? true : false,
        container, stats,
        camera, controls, scene, renderer,
        mesh,
        clock = new THREE.Clock();

    retrieveUniformsLocally();

    navigator.webkitGetUserMedia({ video: true, audio: true }, function(stream){
        video.src    = webkitURL.createObjectURL(stream);
    }, function(error){
        console.log("Failed to get a stream due to", error);
    });
    video.width    = 1280;
    video.height   = 720;
    video.autoplay = true;
    
    init();
    animate();

    function initAudio() {
        
    }

    function init() {
        initAudio();

        container = document.getElementById( 'container' );

        //camera = new THREE.PerspectiveCamera( 60, window.innerWidth / window.innerHeight, 1, 20000 );
        camera = new THREE.PerspectiveCamera(75, window.innerWidth / window.innerHeight, 0.1, 1000);
        scene = new THREE.Scene();

        // sides
        var matrix = new THREE.Matrix4();
        videoTexture = new THREE.Texture( video );
        uniforms.texture1 = { type: "t", value: videoTexture };

        var backgroundMesh = new THREE.Mesh(
	    new THREE.PlaneGeometry(160, 90, 160, 90),
	    new THREE.ShaderMaterial({
	        //wireframe: true,	            	
	        uniforms: uniforms,
	        vertexShader: document.getElementById( 'vertexShader' ).textContent,
	        fragmentShader: document.getElementById( 'fragmentShader' ).textContent
	    })
	    //new THREE.MeshBasicMaterial({
	    //map: videoTexture
	    //})
        );

        backgroundMesh .material.depthTest = false;
        backgroundMesh .material.depthWrite = false;

        camera.rotation.y = 0 * Math.PI / 180;
        camera.position.z = 120;
        camera.position.x = 0;

        scene.add( backgroundMesh );

        var ambientLight = new THREE.AmbientLight( 0xcccccc );
        scene.add( ambientLight );

        renderer = new THREE.WebGLRenderer();
        renderer.setClearColor( 0x000000, 1 );
        renderer.setSize( window.innerWidth, window.innerHeight );

        // Here is the effect for the Oculus Rift
        // worldScale 100 means that 100 Units == 1m
        effect = new THREE.OculusRiftEffect( renderer, {worldScale: 100} );
        //effect = new THREE.StereoEffect( renderer );
        //effect.separation = 1;
        
        effect.setSize( window.innerWidth, window.innerHeight );

        container.innerHTML = "";

        container.appendChild( renderer.domElement );
        /*
          stats = new Stats();
          stats.domElement.style.position = 'absolute';
          stats.domElement.style.top = '0px';
          container.appendChild( stats.domElement );
        */

        // GUI
        window.addEventListener( 'resize', onWindowResize, false );
        document.addEventListener( 'keydown', keyPressed, false );

        guiVisible = true;

    }

    function onWindowResize() {
        camera.aspect = window.innerWidth / window.innerHeight;
        renderer.setSize( window.innerWidth, window.innerHeight );
        effect.setSize( window.innerWidth, window.innerHeight );
        //effect.setSize( window.innerWidth, window.innerHeight );

        camera.updateProjectionMatrix();
        

    }

    function keyPressed (event) {
        if (event.keyCode === 72) { // H
	    guiVisible = !guiVisible;
	    //document.getElementById('info').style.display = guiVisible ? "block" : "none";
	    //stats.domElement.style.display = guiVisible ? "block" : "none";
        }
    }

    function animate() {
        if( video.readyState === video.HAVE_ENOUGH_DATA ){
	    videoTexture.needsUpdate = true;
        }

        //camera.rotation.y = ( new Date().getTime()) * 0.04 * Math.PI / 180;

        requestAnimationFrame( animate );

        render();
    }

    function render() {
        //clock.getDelta() 
        effect.render( scene, camera );
        //renderer.render(scene,camera);
    }

    // Event listeners
    function onKeyDown(event){
        console.log("onKeyDown");
        
        var curvatureMutliplier = 1.05;
        var curavatureReciprocal = 1 / curvatureMutliplier;

        if( event.keyCode == 38 ){
	    uniforms.curvatureScale.value += 1;
        }
        if( event.keyCode == 40 ){
	    uniforms.curvatureScale.value -= 1;
        }
        if( event.keyCode == 37 ){
	    uniforms.curvature.value.x *= curavatureReciprocal;
        }
        if( event.keyCode == 39 ){
	    uniforms.curvature.value.x *= curvatureMutliplier;
        }
        uniforms.curvature.value.x = Math.min(1,uniforms.curvature.value.x);
        uniforms.curvature.value.y = uniforms.curvature.value.x * 0.5625;
        storeUniformsLocally();
        
    }


    function enterFullscreen() {
        var elem = document.body;
        //elem.onwebkitfullscreenchange = onFullScreenEnter;
        //elem.onmozfullscreenchange = onFullScreenEnter;
        //elem.onfullscreenchange = onFullScreenEnter;
        if (elem.webkitRequestFullscreen) {
	    elem.webkitRequestFullscreen(Element.ALLOW_KEYBOARD_INPUT);
        } else {
	    if (elem.mozRequestFullScreen) {
	        elem.mozRequestFullScreen();
	    } else {
	        elem.requestFullscreen();
	    }
        }
    }

    document.addEventListener('keydown', function(e) {
        switch (e.keyCode) {
        case 13: // ENTER. ESC should also take you out of fullscreen by default.
	    e.preventDefault();
	    document.cancelFullScreen(); // explicitly go out of fs.
	    break;
        case 70: // f
	    enterFullscreen();
	    break;
        }
    }, false);

    window.addEventListener('keydown', onKeyDown );

})();

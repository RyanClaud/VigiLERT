const __vite__mapDeps=(i,m=__vite__mapDeps,d=(m.f||(m.f=["assets/Dashboard-BWv6PRyz.js","assets/RecentAlerts-Cj4roK8Z.js","assets/Dashboard-Dgihpmma.css","assets/EmergencyContactDashboard-CL4KWl9d.js"])))=>i.map(i=>d[i]);
(function(){const e=document.createElement("link").relList;if(e&&e.supports&&e.supports("modulepreload"))return;for(const i of document.querySelectorAll('link[rel="modulepreload"]'))s(i);new MutationObserver(i=>{for(const r of i)if(r.type==="childList")for(const o of r.addedNodes)o.tagName==="LINK"&&o.rel==="modulepreload"&&s(o)}).observe(document,{childList:!0,subtree:!0});function n(i){const r={};return i.integrity&&(r.integrity=i.integrity),i.referrerPolicy&&(r.referrerPolicy=i.referrerPolicy),i.crossOrigin==="use-credentials"?r.credentials="include":i.crossOrigin==="anonymous"?r.credentials="omit":r.credentials="same-origin",r}function s(i){if(i.ep)return;i.ep=!0;const r=n(i);fetch(i.href,r)}})();/**
* @vue/shared v3.5.13
* (c) 2018-present Yuxi (Evan) You and Vue contributors
* @license MIT
**//*! #__NO_SIDE_EFFECTS__ */function ga(t){const e=Object.create(null);for(const n of t.split(","))e[n]=1;return n=>n in e}const ae={},Zn=[],vt=()=>{},Lp=()=>!1,Pr=t=>t.charCodeAt(0)===111&&t.charCodeAt(1)===110&&(t.charCodeAt(2)>122||t.charCodeAt(2)<97),ma=t=>t.startsWith("onUpdate:"),ke=Object.assign,va=(t,e)=>{const n=t.indexOf(e);n>-1&&t.splice(n,1)},Fp=Object.prototype.hasOwnProperty,se=(t,e)=>Fp.call(t,e),H=Array.isArray,es=t=>gi(t)==="[object Map]",Ar=t=>gi(t)==="[object Set]",Ml=t=>gi(t)==="[object Date]",$=t=>typeof t=="function",ye=t=>typeof t=="string",Et=t=>typeof t=="symbol",de=t=>t!==null&&typeof t=="object",Gu=t=>(de(t)||$(t))&&$(t.then)&&$(t.catch),Ku=Object.prototype.toString,gi=t=>Ku.call(t),Up=t=>gi(t).slice(8,-1),qu=t=>gi(t)==="[object Object]",ya=t=>ye(t)&&t!=="NaN"&&t[0]!=="-"&&""+parseInt(t,10)===t,Ms=ga(",key,ref,ref_for,ref_key,onVnodeBeforeMount,onVnodeMounted,onVnodeBeforeUpdate,onVnodeUpdated,onVnodeBeforeUnmount,onVnodeUnmounted"),Nr=t=>{const e=Object.create(null);return n=>e[n]||(e[n]=t(n))},Wp=/-(\w)/g,tt=Nr(t=>t.replace(Wp,(e,n)=>n?n.toUpperCase():"")),Hp=/\B([A-Z])/g,Bn=Nr(t=>t.replace(Hp,"-$1").toLowerCase()),kr=Nr(t=>t.charAt(0).toUpperCase()+t.slice(1)),oo=Nr(t=>t?`on${kr(t)}`:""),nn=(t,e)=>!Object.is(t,e),Ui=(t,...e)=>{for(let n=0;n<t.length;n++)t[n](...e)},zu=(t,e,n,s=!1)=>{Object.defineProperty(t,e,{configurable:!0,enumerable:!1,writable:s,value:n})},zi=t=>{const e=parseFloat(t);return isNaN(e)?t:e};let Ll;const Or=()=>Ll||(Ll=typeof globalThis<"u"?globalThis:typeof self<"u"?self:typeof window<"u"?window:typeof global<"u"?global:{});function Ea(t){if(H(t)){const e={};for(let n=0;n<t.length;n++){const s=t[n],i=ye(s)?jp(s):Ea(s);if(i)for(const r in i)e[r]=i[r]}return e}else if(ye(t)||de(t))return t}const Vp=/;(?![^(]*\))/g,Bp=/:([^]+)/,$p=/\/\*[^]*?\*\//g;function jp(t){const e={};return t.replace($p,"").split(Vp).forEach(n=>{if(n){const s=n.split(Bp);s.length>1&&(e[s[0].trim()]=s[1].trim())}}),e}function wa(t){let e="";if(ye(t))e=t;else if(H(t))for(let n=0;n<t.length;n++){const s=wa(t[n]);s&&(e+=s+" ")}else if(de(t))for(const n in t)t[n]&&(e+=n+" ");return e.trim()}const Gp="itemscope,allowfullscreen,formnovalidate,ismap,nomodule,novalidate,readonly",Kp=ga(Gp);function Yu(t){return!!t||t===""}function qp(t,e){if(t.length!==e.length)return!1;let n=!0;for(let s=0;n&&s<t.length;s++)n=xr(t[s],e[s]);return n}function xr(t,e){if(t===e)return!0;let n=Ml(t),s=Ml(e);if(n||s)return n&&s?t.getTime()===e.getTime():!1;if(n=Et(t),s=Et(e),n||s)return t===e;if(n=H(t),s=H(e),n||s)return n&&s?qp(t,e):!1;if(n=de(t),s=de(e),n||s){if(!n||!s)return!1;const i=Object.keys(t).length,r=Object.keys(e).length;if(i!==r)return!1;for(const o in t){const a=t.hasOwnProperty(o),l=e.hasOwnProperty(o);if(a&&!l||!a&&l||!xr(t[o],e[o]))return!1}}return String(t)===String(e)}function zp(t,e){return t.findIndex(n=>xr(n,e))}const Qu=t=>!!(t&&t.__v_isRef===!0),Yp=t=>ye(t)?t:t==null?"":H(t)||de(t)&&(t.toString===Ku||!$(t.toString))?Qu(t)?Yp(t.value):JSON.stringify(t,Ju,2):String(t),Ju=(t,e)=>Qu(e)?Ju(t,e.value):es(e)?{[`Map(${e.size})`]:[...e.entries()].reduce((n,[s,i],r)=>(n[ao(s,r)+" =>"]=i,n),{})}:Ar(e)?{[`Set(${e.size})`]:[...e.values()].map(n=>ao(n))}:Et(e)?ao(e):de(e)&&!H(e)&&!qu(e)?String(e):e,ao=(t,e="")=>{var n;return Et(t)?`Symbol(${(n=t.description)!=null?n:e})`:t};/**
* @vue/reactivity v3.5.13
* (c) 2018-present Yuxi (Evan) You and Vue contributors
* @license MIT
**/let De;class Xu{constructor(e=!1){this.detached=e,this._active=!0,this.effects=[],this.cleanups=[],this._isPaused=!1,this.parent=De,!e&&De&&(this.index=(De.scopes||(De.scopes=[])).push(this)-1)}get active(){return this._active}pause(){if(this._active){this._isPaused=!0;let e,n;if(this.scopes)for(e=0,n=this.scopes.length;e<n;e++)this.scopes[e].pause();for(e=0,n=this.effects.length;e<n;e++)this.effects[e].pause()}}resume(){if(this._active&&this._isPaused){this._isPaused=!1;let e,n;if(this.scopes)for(e=0,n=this.scopes.length;e<n;e++)this.scopes[e].resume();for(e=0,n=this.effects.length;e<n;e++)this.effects[e].resume()}}run(e){if(this._active){const n=De;try{return De=this,e()}finally{De=n}}}on(){De=this}off(){De=this.parent}stop(e){if(this._active){this._active=!1;let n,s;for(n=0,s=this.effects.length;n<s;n++)this.effects[n].stop();for(this.effects.length=0,n=0,s=this.cleanups.length;n<s;n++)this.cleanups[n]();if(this.cleanups.length=0,this.scopes){for(n=0,s=this.scopes.length;n<s;n++)this.scopes[n].stop(!0);this.scopes.length=0}if(!this.detached&&this.parent&&!e){const i=this.parent.scopes.pop();i&&i!==this&&(this.parent.scopes[this.index]=i,i.index=this.index)}this.parent=void 0}}}function Zu(t){return new Xu(t)}function eh(){return De}function Qp(t,e=!1){De&&De.cleanups.push(t)}let ue;const lo=new WeakSet;class th{constructor(e){this.fn=e,this.deps=void 0,this.depsTail=void 0,this.flags=5,this.next=void 0,this.cleanup=void 0,this.scheduler=void 0,De&&De.active&&De.effects.push(this)}pause(){this.flags|=64}resume(){this.flags&64&&(this.flags&=-65,lo.has(this)&&(lo.delete(this),this.trigger()))}notify(){this.flags&2&&!(this.flags&32)||this.flags&8||sh(this)}run(){if(!(this.flags&1))return this.fn();this.flags|=2,Fl(this),ih(this);const e=ue,n=at;ue=this,at=!0;try{return this.fn()}finally{rh(this),ue=e,at=n,this.flags&=-3}}stop(){if(this.flags&1){for(let e=this.deps;e;e=e.nextDep)Ca(e);this.deps=this.depsTail=void 0,Fl(this),this.onStop&&this.onStop(),this.flags&=-2}}trigger(){this.flags&64?lo.add(this):this.scheduler?this.scheduler():this.runIfDirty()}runIfDirty(){Do(this)&&this.run()}get dirty(){return Do(this)}}let nh=0,Ls,Fs;function sh(t,e=!1){if(t.flags|=8,e){t.next=Fs,Fs=t;return}t.next=Ls,Ls=t}function Ia(){nh++}function ba(){if(--nh>0)return;if(Fs){let e=Fs;for(Fs=void 0;e;){const n=e.next;e.next=void 0,e.flags&=-9,e=n}}let t;for(;Ls;){let e=Ls;for(Ls=void 0;e;){const n=e.next;if(e.next=void 0,e.flags&=-9,e.flags&1)try{e.trigger()}catch(s){t||(t=s)}e=n}}if(t)throw t}function ih(t){for(let e=t.deps;e;e=e.nextDep)e.version=-1,e.prevActiveLink=e.dep.activeLink,e.dep.activeLink=e}function rh(t){let e,n=t.depsTail,s=n;for(;s;){const i=s.prevDep;s.version===-1?(s===n&&(n=i),Ca(s),Jp(s)):e=s,s.dep.activeLink=s.prevActiveLink,s.prevActiveLink=void 0,s=i}t.deps=e,t.depsTail=n}function Do(t){for(let e=t.deps;e;e=e.nextDep)if(e.dep.version!==e.version||e.dep.computed&&(oh(e.dep.computed)||e.dep.version!==e.version))return!0;return!!t._dirty}function oh(t){if(t.flags&4&&!(t.flags&16)||(t.flags&=-17,t.globalVersion===Qs))return;t.globalVersion=Qs;const e=t.dep;if(t.flags|=2,e.version>0&&!t.isSSR&&t.deps&&!Do(t)){t.flags&=-3;return}const n=ue,s=at;ue=t,at=!0;try{ih(t);const i=t.fn(t._value);(e.version===0||nn(i,t._value))&&(t._value=i,e.version++)}catch(i){throw e.version++,i}finally{ue=n,at=s,rh(t),t.flags&=-3}}function Ca(t,e=!1){const{dep:n,prevSub:s,nextSub:i}=t;if(s&&(s.nextSub=i,t.prevSub=void 0),i&&(i.prevSub=s,t.nextSub=void 0),n.subs===t&&(n.subs=s,!s&&n.computed)){n.computed.flags&=-5;for(let r=n.computed.deps;r;r=r.nextDep)Ca(r,!0)}!e&&!--n.sc&&n.map&&n.map.delete(n.key)}function Jp(t){const{prevDep:e,nextDep:n}=t;e&&(e.nextDep=n,t.prevDep=void 0),n&&(n.prevDep=e,t.nextDep=void 0)}let at=!0;const ah=[];function pn(){ah.push(at),at=!1}function _n(){const t=ah.pop();at=t===void 0?!0:t}function Fl(t){const{cleanup:e}=t;if(t.cleanup=void 0,e){const n=ue;ue=void 0;try{e()}finally{ue=n}}}let Qs=0;class Xp{constructor(e,n){this.sub=e,this.dep=n,this.version=n.version,this.nextDep=this.prevDep=this.nextSub=this.prevSub=this.prevActiveLink=void 0}}class Sa{constructor(e){this.computed=e,this.version=0,this.activeLink=void 0,this.subs=void 0,this.map=void 0,this.key=void 0,this.sc=0}track(e){if(!ue||!at||ue===this.computed)return;let n=this.activeLink;if(n===void 0||n.sub!==ue)n=this.activeLink=new Xp(ue,this),ue.deps?(n.prevDep=ue.depsTail,ue.depsTail.nextDep=n,ue.depsTail=n):ue.deps=ue.depsTail=n,lh(n);else if(n.version===-1&&(n.version=this.version,n.nextDep)){const s=n.nextDep;s.prevDep=n.prevDep,n.prevDep&&(n.prevDep.nextDep=s),n.prevDep=ue.depsTail,n.nextDep=void 0,ue.depsTail.nextDep=n,ue.depsTail=n,ue.deps===n&&(ue.deps=s)}return n}trigger(e){this.version++,Qs++,this.notify(e)}notify(e){Ia();try{for(let n=this.subs;n;n=n.prevSub)n.sub.notify()&&n.sub.dep.notify()}finally{ba()}}}function lh(t){if(t.dep.sc++,t.sub.flags&4){const e=t.dep.computed;if(e&&!t.dep.subs){e.flags|=20;for(let s=e.deps;s;s=s.nextDep)lh(s)}const n=t.dep.subs;n!==t&&(t.prevSub=n,n&&(n.nextSub=t)),t.dep.subs=t}}const Yi=new WeakMap,On=Symbol(""),Mo=Symbol(""),Js=Symbol("");function Pe(t,e,n){if(at&&ue){let s=Yi.get(t);s||Yi.set(t,s=new Map);let i=s.get(n);i||(s.set(n,i=new Sa),i.map=s,i.key=n),i.track()}}function At(t,e,n,s,i,r){const o=Yi.get(t);if(!o){Qs++;return}const a=l=>{l&&l.trigger()};if(Ia(),e==="clear")o.forEach(a);else{const l=H(t),c=l&&ya(n);if(l&&n==="length"){const h=Number(s);o.forEach((u,d)=>{(d==="length"||d===Js||!Et(d)&&d>=h)&&a(u)})}else switch((n!==void 0||o.has(void 0))&&a(o.get(n)),c&&a(o.get(Js)),e){case"add":l?c&&a(o.get("length")):(a(o.get(On)),es(t)&&a(o.get(Mo)));break;case"delete":l||(a(o.get(On)),es(t)&&a(o.get(Mo)));break;case"set":es(t)&&a(o.get(On));break}}ba()}function Zp(t,e){const n=Yi.get(t);return n&&n.get(e)}function Kn(t){const e=Z(t);return e===t?e:(Pe(e,"iterate",Js),Ze(t)?e:e.map(Ae))}function Dr(t){return Pe(t=Z(t),"iterate",Js),t}const e_={__proto__:null,[Symbol.iterator](){return co(this,Symbol.iterator,Ae)},concat(...t){return Kn(this).concat(...t.map(e=>H(e)?Kn(e):e))},entries(){return co(this,"entries",t=>(t[1]=Ae(t[1]),t))},every(t,e){return Ct(this,"every",t,e,void 0,arguments)},filter(t,e){return Ct(this,"filter",t,e,n=>n.map(Ae),arguments)},find(t,e){return Ct(this,"find",t,e,Ae,arguments)},findIndex(t,e){return Ct(this,"findIndex",t,e,void 0,arguments)},findLast(t,e){return Ct(this,"findLast",t,e,Ae,arguments)},findLastIndex(t,e){return Ct(this,"findLastIndex",t,e,void 0,arguments)},forEach(t,e){return Ct(this,"forEach",t,e,void 0,arguments)},includes(...t){return uo(this,"includes",t)},indexOf(...t){return uo(this,"indexOf",t)},join(t){return Kn(this).join(t)},lastIndexOf(...t){return uo(this,"lastIndexOf",t)},map(t,e){return Ct(this,"map",t,e,void 0,arguments)},pop(){return Ss(this,"pop")},push(...t){return Ss(this,"push",t)},reduce(t,...e){return Ul(this,"reduce",t,e)},reduceRight(t,...e){return Ul(this,"reduceRight",t,e)},shift(){return Ss(this,"shift")},some(t,e){return Ct(this,"some",t,e,void 0,arguments)},splice(...t){return Ss(this,"splice",t)},toReversed(){return Kn(this).toReversed()},toSorted(t){return Kn(this).toSorted(t)},toSpliced(...t){return Kn(this).toSpliced(...t)},unshift(...t){return Ss(this,"unshift",t)},values(){return co(this,"values",Ae)}};function co(t,e,n){const s=Dr(t),i=s[e]();return s!==t&&!Ze(t)&&(i._next=i.next,i.next=()=>{const r=i._next();return r.value&&(r.value=n(r.value)),r}),i}const t_=Array.prototype;function Ct(t,e,n,s,i,r){const o=Dr(t),a=o!==t&&!Ze(t),l=o[e];if(l!==t_[e]){const u=l.apply(t,r);return a?Ae(u):u}let c=n;o!==t&&(a?c=function(u,d){return n.call(this,Ae(u),d,t)}:n.length>2&&(c=function(u,d){return n.call(this,u,d,t)}));const h=l.call(o,c,s);return a&&i?i(h):h}function Ul(t,e,n,s){const i=Dr(t);let r=n;return i!==t&&(Ze(t)?n.length>3&&(r=function(o,a,l){return n.call(this,o,a,l,t)}):r=function(o,a,l){return n.call(this,o,Ae(a),l,t)}),i[e](r,...s)}function uo(t,e,n){const s=Z(t);Pe(s,"iterate",Js);const i=s[e](...n);return(i===-1||i===!1)&&Pa(n[0])?(n[0]=Z(n[0]),s[e](...n)):i}function Ss(t,e,n=[]){pn(),Ia();const s=Z(t)[e].apply(t,n);return ba(),_n(),s}const n_=ga("__proto__,__v_isRef,__isVue"),ch=new Set(Object.getOwnPropertyNames(Symbol).filter(t=>t!=="arguments"&&t!=="caller").map(t=>Symbol[t]).filter(Et));function s_(t){Et(t)||(t=String(t));const e=Z(this);return Pe(e,"has",t),e.hasOwnProperty(t)}class uh{constructor(e=!1,n=!1){this._isReadonly=e,this._isShallow=n}get(e,n,s){if(n==="__v_skip")return e.__v_skip;const i=this._isReadonly,r=this._isShallow;if(n==="__v_isReactive")return!i;if(n==="__v_isReadonly")return i;if(n==="__v_isShallow")return r;if(n==="__v_raw")return s===(i?r?f_:ph:r?fh:dh).get(e)||Object.getPrototypeOf(e)===Object.getPrototypeOf(s)?e:void 0;const o=H(e);if(!i){let l;if(o&&(l=e_[n]))return l;if(n==="hasOwnProperty")return s_}const a=Reflect.get(e,n,ve(e)?e:s);return(Et(n)?ch.has(n):n_(n))||(i||Pe(e,"get",n),r)?a:ve(a)?o&&ya(n)?a:a.value:de(a)?i?gh(a):mi(a):a}}class hh extends uh{constructor(e=!1){super(!1,e)}set(e,n,s,i){let r=e[n];if(!this._isShallow){const l=Dn(r);if(!Ze(s)&&!Dn(s)&&(r=Z(r),s=Z(s)),!H(e)&&ve(r)&&!ve(s))return l?!1:(r.value=s,!0)}const o=H(e)&&ya(n)?Number(n)<e.length:se(e,n),a=Reflect.set(e,n,s,ve(e)?e:i);return e===Z(i)&&(o?nn(s,r)&&At(e,"set",n,s):At(e,"add",n,s)),a}deleteProperty(e,n){const s=se(e,n);e[n];const i=Reflect.deleteProperty(e,n);return i&&s&&At(e,"delete",n,void 0),i}has(e,n){const s=Reflect.has(e,n);return(!Et(n)||!ch.has(n))&&Pe(e,"has",n),s}ownKeys(e){return Pe(e,"iterate",H(e)?"length":On),Reflect.ownKeys(e)}}class i_ extends uh{constructor(e=!1){super(!0,e)}set(e,n){return!0}deleteProperty(e,n){return!0}}const r_=new hh,o_=new i_,a_=new hh(!0);const Lo=t=>t,Oi=t=>Reflect.getPrototypeOf(t);function l_(t,e,n){return function(...s){const i=this.__v_raw,r=Z(i),o=es(r),a=t==="entries"||t===Symbol.iterator&&o,l=t==="keys"&&o,c=i[t](...s),h=n?Lo:e?Fo:Ae;return!e&&Pe(r,"iterate",l?Mo:On),{next(){const{value:u,done:d}=c.next();return d?{value:u,done:d}:{value:a?[h(u[0]),h(u[1])]:h(u),done:d}},[Symbol.iterator](){return this}}}}function xi(t){return function(...e){return t==="delete"?!1:t==="clear"?void 0:this}}function c_(t,e){const n={get(i){const r=this.__v_raw,o=Z(r),a=Z(i);t||(nn(i,a)&&Pe(o,"get",i),Pe(o,"get",a));const{has:l}=Oi(o),c=e?Lo:t?Fo:Ae;if(l.call(o,i))return c(r.get(i));if(l.call(o,a))return c(r.get(a));r!==o&&r.get(i)},get size(){const i=this.__v_raw;return!t&&Pe(Z(i),"iterate",On),Reflect.get(i,"size",i)},has(i){const r=this.__v_raw,o=Z(r),a=Z(i);return t||(nn(i,a)&&Pe(o,"has",i),Pe(o,"has",a)),i===a?r.has(i):r.has(i)||r.has(a)},forEach(i,r){const o=this,a=o.__v_raw,l=Z(a),c=e?Lo:t?Fo:Ae;return!t&&Pe(l,"iterate",On),a.forEach((h,u)=>i.call(r,c(h),c(u),o))}};return ke(n,t?{add:xi("add"),set:xi("set"),delete:xi("delete"),clear:xi("clear")}:{add(i){!e&&!Ze(i)&&!Dn(i)&&(i=Z(i));const r=Z(this);return Oi(r).has.call(r,i)||(r.add(i),At(r,"add",i,i)),this},set(i,r){!e&&!Ze(r)&&!Dn(r)&&(r=Z(r));const o=Z(this),{has:a,get:l}=Oi(o);let c=a.call(o,i);c||(i=Z(i),c=a.call(o,i));const h=l.call(o,i);return o.set(i,r),c?nn(r,h)&&At(o,"set",i,r):At(o,"add",i,r),this},delete(i){const r=Z(this),{has:o,get:a}=Oi(r);let l=o.call(r,i);l||(i=Z(i),l=o.call(r,i)),a&&a.call(r,i);const c=r.delete(i);return l&&At(r,"delete",i,void 0),c},clear(){const i=Z(this),r=i.size!==0,o=i.clear();return r&&At(i,"clear",void 0,void 0),o}}),["keys","values","entries",Symbol.iterator].forEach(i=>{n[i]=l_(i,t,e)}),n}function Ta(t,e){const n=c_(t,e);return(s,i,r)=>i==="__v_isReactive"?!t:i==="__v_isReadonly"?t:i==="__v_raw"?s:Reflect.get(se(n,i)&&i in s?n:s,i,r)}const u_={get:Ta(!1,!1)},h_={get:Ta(!1,!0)},d_={get:Ta(!0,!1)};const dh=new WeakMap,fh=new WeakMap,ph=new WeakMap,f_=new WeakMap;function p_(t){switch(t){case"Object":case"Array":return 1;case"Map":case"Set":case"WeakMap":case"WeakSet":return 2;default:return 0}}function __(t){return t.__v_skip||!Object.isExtensible(t)?0:p_(Up(t))}function mi(t){return Dn(t)?t:Ra(t,!1,r_,u_,dh)}function _h(t){return Ra(t,!1,a_,h_,fh)}function gh(t){return Ra(t,!0,o_,d_,ph)}function Ra(t,e,n,s,i){if(!de(t)||t.__v_raw&&!(e&&t.__v_isReactive))return t;const r=i.get(t);if(r)return r;const o=__(t);if(o===0)return t;const a=new Proxy(t,o===2?s:n);return i.set(t,a),a}function sn(t){return Dn(t)?sn(t.__v_raw):!!(t&&t.__v_isReactive)}function Dn(t){return!!(t&&t.__v_isReadonly)}function Ze(t){return!!(t&&t.__v_isShallow)}function Pa(t){return t?!!t.__v_raw:!1}function Z(t){const e=t&&t.__v_raw;return e?Z(e):t}function Aa(t){return!se(t,"__v_skip")&&Object.isExtensible(t)&&zu(t,"__v_skip",!0),t}const Ae=t=>de(t)?mi(t):t,Fo=t=>de(t)?gh(t):t;function ve(t){return t?t.__v_isRef===!0:!1}function Mr(t){return mh(t,!1)}function g_(t){return mh(t,!0)}function mh(t,e){return ve(t)?t:new m_(t,e)}class m_{constructor(e,n){this.dep=new Sa,this.__v_isRef=!0,this.__v_isShallow=!1,this._rawValue=n?e:Z(e),this._value=n?e:Ae(e),this.__v_isShallow=n}get value(){return this.dep.track(),this._value}set value(e){const n=this._rawValue,s=this.__v_isShallow||Ze(e)||Dn(e);e=s?e:Z(e),nn(e,n)&&(this._rawValue=e,this._value=s?e:Ae(e),this.dep.trigger())}}function rn(t){return ve(t)?t.value:t}const v_={get:(t,e,n)=>e==="__v_raw"?t:rn(Reflect.get(t,e,n)),set:(t,e,n,s)=>{const i=t[e];return ve(i)&&!ve(n)?(i.value=n,!0):Reflect.set(t,e,n,s)}};function vh(t){return sn(t)?t:new Proxy(t,v_)}function y_(t){const e=H(t)?new Array(t.length):{};for(const n in t)e[n]=w_(t,n);return e}class E_{constructor(e,n,s){this._object=e,this._key=n,this._defaultValue=s,this.__v_isRef=!0,this._value=void 0}get value(){const e=this._object[this._key];return this._value=e===void 0?this._defaultValue:e}set value(e){this._object[this._key]=e}get dep(){return Zp(Z(this._object),this._key)}}function w_(t,e,n){const s=t[e];return ve(s)?s:new E_(t,e,n)}class I_{constructor(e,n,s){this.fn=e,this.setter=n,this._value=void 0,this.dep=new Sa(this),this.__v_isRef=!0,this.deps=void 0,this.depsTail=void 0,this.flags=16,this.globalVersion=Qs-1,this.next=void 0,this.effect=this,this.__v_isReadonly=!n,this.isSSR=s}notify(){if(this.flags|=16,!(this.flags&8)&&ue!==this)return sh(this,!0),!0}get value(){const e=this.dep.track();return oh(this),e&&(e.version=this.dep.version),this._value}set value(e){this.setter&&this.setter(e)}}function b_(t,e,n=!1){let s,i;return $(t)?s=t:(s=t.get,i=t.set),new I_(s,i,n)}const Di={},Qi=new WeakMap;let bn;function C_(t,e=!1,n=bn){if(n){let s=Qi.get(n);s||Qi.set(n,s=[]),s.push(t)}}function S_(t,e,n=ae){const{immediate:s,deep:i,once:r,scheduler:o,augmentJob:a,call:l}=n,c=x=>i?x:Ze(x)||i===!1||i===0?Nt(x,1):Nt(x);let h,u,d,_,m=!1,I=!1;if(ve(t)?(u=()=>t.value,m=Ze(t)):sn(t)?(u=()=>c(t),m=!0):H(t)?(I=!0,m=t.some(x=>sn(x)||Ze(x)),u=()=>t.map(x=>{if(ve(x))return x.value;if(sn(x))return c(x);if($(x))return l?l(x,2):x()})):$(t)?e?u=l?()=>l(t,2):t:u=()=>{if(d){pn();try{d()}finally{_n()}}const x=bn;bn=h;try{return l?l(t,3,[_]):t(_)}finally{bn=x}}:u=vt,e&&i){const x=u,j=i===!0?1/0:i;u=()=>Nt(x(),j)}const O=eh(),D=()=>{h.stop(),O&&O.active&&va(O.effects,h)};if(r&&e){const x=e;e=(...j)=>{x(...j),D()}}let k=I?new Array(t.length).fill(Di):Di;const M=x=>{if(!(!(h.flags&1)||!h.dirty&&!x))if(e){const j=h.run();if(i||m||(I?j.some((ge,X)=>nn(ge,k[X])):nn(j,k))){d&&d();const ge=bn;bn=h;try{const X=[j,k===Di?void 0:I&&k[0]===Di?[]:k,_];l?l(e,3,X):e(...X),k=j}finally{bn=ge}}}else h.run()};return a&&a(M),h=new th(u),h.scheduler=o?()=>o(M,!1):M,_=x=>C_(x,!1,h),d=h.onStop=()=>{const x=Qi.get(h);if(x){if(l)l(x,4);else for(const j of x)j();Qi.delete(h)}},e?s?M(!0):k=h.run():o?o(M.bind(null,!0),!0):h.run(),D.pause=h.pause.bind(h),D.resume=h.resume.bind(h),D.stop=D,D}function Nt(t,e=1/0,n){if(e<=0||!de(t)||t.__v_skip||(n=n||new Set,n.has(t)))return t;if(n.add(t),e--,ve(t))Nt(t.value,e,n);else if(H(t))for(let s=0;s<t.length;s++)Nt(t[s],e,n);else if(Ar(t)||es(t))t.forEach(s=>{Nt(s,e,n)});else if(qu(t)){for(const s in t)Nt(t[s],e,n);for(const s of Object.getOwnPropertySymbols(t))Object.prototype.propertyIsEnumerable.call(t,s)&&Nt(t[s],e,n)}return t}/**
* @vue/runtime-core v3.5.13
* (c) 2018-present Yuxi (Evan) You and Vue contributors
* @license MIT
**/function vi(t,e,n,s){try{return s?t(...s):t()}catch(i){Lr(i,e,n)}}function wt(t,e,n,s){if($(t)){const i=vi(t,e,n,s);return i&&Gu(i)&&i.catch(r=>{Lr(r,e,n)}),i}if(H(t)){const i=[];for(let r=0;r<t.length;r++)i.push(wt(t[r],e,n,s));return i}}function Lr(t,e,n,s=!0){const i=e?e.vnode:null,{errorHandler:r,throwUnhandledErrorInProduction:o}=e&&e.appContext.config||ae;if(e){let a=e.parent;const l=e.proxy,c=`https://vuejs.org/error-reference/#runtime-${n}`;for(;a;){const h=a.ec;if(h){for(let u=0;u<h.length;u++)if(h[u](t,l,c)===!1)return}a=a.parent}if(r){pn(),vi(r,null,10,[t,l,c]),_n();return}}T_(t,n,i,s,o)}function T_(t,e,n,s=!0,i=!1){if(i)throw t;console.error(t)}const Me=[];let gt=-1;const ts=[];let Yt=null,Qn=0;const yh=Promise.resolve();let Ji=null;function Fr(t){const e=Ji||yh;return t?e.then(this?t.bind(this):t):e}function R_(t){let e=gt+1,n=Me.length;for(;e<n;){const s=e+n>>>1,i=Me[s],r=Xs(i);r<t||r===t&&i.flags&2?e=s+1:n=s}return e}function Na(t){if(!(t.flags&1)){const e=Xs(t),n=Me[Me.length-1];!n||!(t.flags&2)&&e>=Xs(n)?Me.push(t):Me.splice(R_(e),0,t),t.flags|=1,Eh()}}function Eh(){Ji||(Ji=yh.then(Ih))}function P_(t){H(t)?ts.push(...t):Yt&&t.id===-1?Yt.splice(Qn+1,0,t):t.flags&1||(ts.push(t),t.flags|=1),Eh()}function Wl(t,e,n=gt+1){for(;n<Me.length;n++){const s=Me[n];if(s&&s.flags&2){if(t&&s.id!==t.uid)continue;Me.splice(n,1),n--,s.flags&4&&(s.flags&=-2),s(),s.flags&4||(s.flags&=-2)}}}function wh(t){if(ts.length){const e=[...new Set(ts)].sort((n,s)=>Xs(n)-Xs(s));if(ts.length=0,Yt){Yt.push(...e);return}for(Yt=e,Qn=0;Qn<Yt.length;Qn++){const n=Yt[Qn];n.flags&4&&(n.flags&=-2),n.flags&8||n(),n.flags&=-2}Yt=null,Qn=0}}const Xs=t=>t.id==null?t.flags&2?-1:1/0:t.id;function Ih(t){try{for(gt=0;gt<Me.length;gt++){const e=Me[gt];e&&!(e.flags&8)&&(e.flags&4&&(e.flags&=-2),vi(e,e.i,e.i?15:14),e.flags&4||(e.flags&=-2))}}finally{for(;gt<Me.length;gt++){const e=Me[gt];e&&(e.flags&=-2)}gt=-1,Me.length=0,wh(),Ji=null,(Me.length||ts.length)&&Ih()}}let Be=null,bh=null;function Xi(t){const e=Be;return Be=t,bh=t&&t.type.__scopeId||null,e}function Cn(t,e=Be,n){if(!e||t._n)return t;const s=(...i)=>{s._d&&Ql(-1);const r=Xi(e);let o;try{o=t(...i)}finally{Xi(r),s._d&&Ql(1)}return o};return s._n=!0,s._c=!0,s._d=!0,s}function MS(t,e){if(Be===null)return t;const n=Vr(Be),s=t.dirs||(t.dirs=[]);for(let i=0;i<e.length;i++){let[r,o,a,l=ae]=e[i];r&&($(r)&&(r={mounted:r,updated:r}),r.deep&&Nt(o),s.push({dir:r,instance:n,value:o,oldValue:void 0,arg:a,modifiers:l}))}return t}function wn(t,e,n,s){const i=t.dirs,r=e&&e.dirs;for(let o=0;o<i.length;o++){const a=i[o];r&&(a.oldValue=r[o].value);let l=a.dir[s];l&&(pn(),wt(l,n,8,[t.el,a,t,e]),_n())}}const A_=Symbol("_vte"),N_=t=>t.__isTeleport;function ka(t,e){t.shapeFlag&6&&t.component?(t.transition=e,ka(t.component.subTree,e)):t.shapeFlag&128?(t.ssContent.transition=e.clone(t.ssContent),t.ssFallback.transition=e.clone(t.ssFallback)):t.transition=e}/*! #__NO_SIDE_EFFECTS__ */function Ch(t,e){return $(t)?ke({name:t.name},e,{setup:t}):t}function Sh(t){t.ids=[t.ids[0]+t.ids[2]+++"-",0,0]}function Zi(t,e,n,s,i=!1){if(H(t)){t.forEach((m,I)=>Zi(m,e&&(H(e)?e[I]:e),n,s,i));return}if(Us(s)&&!i){s.shapeFlag&512&&s.type.__asyncResolved&&s.component.subTree.component&&Zi(t,e,n,s.component.subTree);return}const r=s.shapeFlag&4?Vr(s.component):s.el,o=i?null:r,{i:a,r:l}=t,c=e&&e.r,h=a.refs===ae?a.refs={}:a.refs,u=a.setupState,d=Z(u),_=u===ae?()=>!1:m=>se(d,m);if(c!=null&&c!==l&&(ye(c)?(h[c]=null,_(c)&&(u[c]=null)):ve(c)&&(c.value=null)),$(l))vi(l,a,12,[o,h]);else{const m=ye(l),I=ve(l);if(m||I){const O=()=>{if(t.f){const D=m?_(l)?u[l]:h[l]:l.value;i?H(D)&&va(D,r):H(D)?D.includes(r)||D.push(r):m?(h[l]=[r],_(l)&&(u[l]=h[l])):(l.value=[r],t.k&&(h[t.k]=l.value))}else m?(h[l]=o,_(l)&&(u[l]=o)):I&&(l.value=o,t.k&&(h[t.k]=o))};o?(O.id=-1,Ye(O,n)):O()}}}Or().requestIdleCallback;Or().cancelIdleCallback;const Us=t=>!!t.type.__asyncLoader,Th=t=>t.type.__isKeepAlive;function k_(t,e){Rh(t,"a",e)}function O_(t,e){Rh(t,"da",e)}function Rh(t,e,n=Re){const s=t.__wdc||(t.__wdc=()=>{let i=n;for(;i;){if(i.isDeactivated)return;i=i.parent}return t()});if(Ur(e,s,n),n){let i=n.parent;for(;i&&i.parent;)Th(i.parent.vnode)&&x_(s,e,n,i),i=i.parent}}function x_(t,e,n,s){const i=Ur(e,t,s,!0);Ph(()=>{va(s[e],i)},n)}function Ur(t,e,n=Re,s=!1){if(n){const i=n[t]||(n[t]=[]),r=e.__weh||(e.__weh=(...o)=>{pn();const a=yi(n),l=wt(e,n,t,o);return a(),_n(),l});return s?i.unshift(r):i.push(r),r}}const Ht=t=>(e,n=Re)=>{(!ei||t==="sp")&&Ur(t,(...s)=>e(...s),n)},D_=Ht("bm"),M_=Ht("m"),L_=Ht("bu"),F_=Ht("u"),U_=Ht("bum"),Ph=Ht("um"),W_=Ht("sp"),H_=Ht("rtg"),V_=Ht("rtc");function B_(t,e=Re){Ur("ec",t,e)}const Ah="components";function Hl(t,e){return kh(Ah,t,!0,e)||t}const Nh=Symbol.for("v-ndc");function LS(t){return ye(t)?kh(Ah,t,!1)||t:t||Nh}function kh(t,e,n=!0,s=!1){const i=Be||Re;if(i){const r=i.type;{const a=Ag(r,!1);if(a&&(a===e||a===tt(e)||a===kr(tt(e))))return r}const o=Vl(i[t]||r[t],e)||Vl(i.appContext[t],e);return!o&&s?r:o}}function Vl(t,e){return t&&(t[e]||t[tt(e)]||t[kr(tt(e))])}function FS(t,e,n,s){let i;const r=n,o=H(t);if(o||ye(t)){const a=o&&sn(t);let l=!1;a&&(l=!Ze(t),t=Dr(t)),i=new Array(t.length);for(let c=0,h=t.length;c<h;c++)i[c]=e(l?Ae(t[c]):t[c],c,void 0,r)}else if(typeof t=="number"){i=new Array(t);for(let a=0;a<t;a++)i[a]=e(a+1,a,void 0,r)}else if(de(t))if(t[Symbol.iterator])i=Array.from(t,(a,l)=>e(a,l,void 0,r));else{const a=Object.keys(t);i=new Array(a.length);for(let l=0,c=a.length;l<c;l++){const h=a[l];i[l]=e(t[h],h,l,r)}}else i=[];return i}const Uo=t=>t?Zh(t)?Vr(t):Uo(t.parent):null,Ws=ke(Object.create(null),{$:t=>t,$el:t=>t.vnode.el,$data:t=>t.data,$props:t=>t.props,$attrs:t=>t.attrs,$slots:t=>t.slots,$refs:t=>t.refs,$parent:t=>Uo(t.parent),$root:t=>Uo(t.root),$host:t=>t.ce,$emit:t=>t.emit,$options:t=>xh(t),$forceUpdate:t=>t.f||(t.f=()=>{Na(t.update)}),$nextTick:t=>t.n||(t.n=Fr.bind(t.proxy)),$watch:t=>ug.bind(t)}),ho=(t,e)=>t!==ae&&!t.__isScriptSetup&&se(t,e),$_={get({_:t},e){if(e==="__v_skip")return!0;const{ctx:n,setupState:s,data:i,props:r,accessCache:o,type:a,appContext:l}=t;let c;if(e[0]!=="$"){const _=o[e];if(_!==void 0)switch(_){case 1:return s[e];case 2:return i[e];case 4:return n[e];case 3:return r[e]}else{if(ho(s,e))return o[e]=1,s[e];if(i!==ae&&se(i,e))return o[e]=2,i[e];if((c=t.propsOptions[0])&&se(c,e))return o[e]=3,r[e];if(n!==ae&&se(n,e))return o[e]=4,n[e];Wo&&(o[e]=0)}}const h=Ws[e];let u,d;if(h)return e==="$attrs"&&Pe(t.attrs,"get",""),h(t);if((u=a.__cssModules)&&(u=u[e]))return u;if(n!==ae&&se(n,e))return o[e]=4,n[e];if(d=l.config.globalProperties,se(d,e))return d[e]},set({_:t},e,n){const{data:s,setupState:i,ctx:r}=t;return ho(i,e)?(i[e]=n,!0):s!==ae&&se(s,e)?(s[e]=n,!0):se(t.props,e)||e[0]==="$"&&e.slice(1)in t?!1:(r[e]=n,!0)},has({_:{data:t,setupState:e,accessCache:n,ctx:s,appContext:i,propsOptions:r}},o){let a;return!!n[o]||t!==ae&&se(t,o)||ho(e,o)||(a=r[0])&&se(a,o)||se(s,o)||se(Ws,o)||se(i.config.globalProperties,o)},defineProperty(t,e,n){return n.get!=null?t._.accessCache[e]=0:se(n,"value")&&this.set(t,e,n.value,null),Reflect.defineProperty(t,e,n)}};function Bl(t){return H(t)?t.reduce((e,n)=>(e[n]=null,e),{}):t}let Wo=!0;function j_(t){const e=xh(t),n=t.proxy,s=t.ctx;Wo=!1,e.beforeCreate&&$l(e.beforeCreate,t,"bc");const{data:i,computed:r,methods:o,watch:a,provide:l,inject:c,created:h,beforeMount:u,mounted:d,beforeUpdate:_,updated:m,activated:I,deactivated:O,beforeDestroy:D,beforeUnmount:k,destroyed:M,unmounted:x,render:j,renderTracked:ge,renderTriggered:X,errorCaptured:q,serverPrefetch:K,expose:fe,inheritAttrs:Oe,components:Ke,directives:We,filters:En}=e;if(c&&G_(c,s,null),o)for(const G in o){const te=o[G];$(te)&&(s[G]=te.bind(n))}if(i){const G=i.call(n,n);de(G)&&(t.data=mi(G))}if(Wo=!0,r)for(const G in r){const te=r[G],bt=$(te)?te.bind(n,n):$(te.get)?te.get.bind(n,n):vt,jt=!$(te)&&$(te.set)?te.set.bind(n):vt,ht=Xe({get:bt,set:jt});Object.defineProperty(s,G,{enumerable:!0,configurable:!0,get:()=>ht.value,set:He=>ht.value=He})}if(a)for(const G in a)Oh(a[G],s,n,G);if(l){const G=$(l)?l.call(n):l;Reflect.ownKeys(G).forEach(te=>{Wi(te,G[te])})}h&&$l(h,t,"c");function _e(G,te){H(te)?te.forEach(bt=>G(bt.bind(n))):te&&G(te.bind(n))}if(_e(D_,u),_e(M_,d),_e(L_,_),_e(F_,m),_e(k_,I),_e(O_,O),_e(B_,q),_e(V_,ge),_e(H_,X),_e(U_,k),_e(Ph,x),_e(W_,K),H(fe))if(fe.length){const G=t.exposed||(t.exposed={});fe.forEach(te=>{Object.defineProperty(G,te,{get:()=>n[te],set:bt=>n[te]=bt})})}else t.exposed||(t.exposed={});j&&t.render===vt&&(t.render=j),Oe!=null&&(t.inheritAttrs=Oe),Ke&&(t.components=Ke),We&&(t.directives=We),K&&Sh(t)}function G_(t,e,n=vt){H(t)&&(t=Ho(t));for(const s in t){const i=t[s];let r;de(i)?"default"in i?r=et(i.from||s,i.default,!0):r=et(i.from||s):r=et(i),ve(r)?Object.defineProperty(e,s,{enumerable:!0,configurable:!0,get:()=>r.value,set:o=>r.value=o}):e[s]=r}}function $l(t,e,n){wt(H(t)?t.map(s=>s.bind(e.proxy)):t.bind(e.proxy),e,n)}function Oh(t,e,n,s){let i=s.includes(".")?qh(n,s):()=>n[s];if(ye(t)){const r=e[t];$(r)&&ns(i,r)}else if($(t))ns(i,t.bind(n));else if(de(t))if(H(t))t.forEach(r=>Oh(r,e,n,s));else{const r=$(t.handler)?t.handler.bind(n):e[t.handler];$(r)&&ns(i,r,t)}}function xh(t){const e=t.type,{mixins:n,extends:s}=e,{mixins:i,optionsCache:r,config:{optionMergeStrategies:o}}=t.appContext,a=r.get(e);let l;return a?l=a:!i.length&&!n&&!s?l=e:(l={},i.length&&i.forEach(c=>er(l,c,o,!0)),er(l,e,o)),de(e)&&r.set(e,l),l}function er(t,e,n,s=!1){const{mixins:i,extends:r}=e;r&&er(t,r,n,!0),i&&i.forEach(o=>er(t,o,n,!0));for(const o in e)if(!(s&&o==="expose")){const a=K_[o]||n&&n[o];t[o]=a?a(t[o],e[o]):e[o]}return t}const K_={data:jl,props:Gl,emits:Gl,methods:Os,computed:Os,beforeCreate:xe,created:xe,beforeMount:xe,mounted:xe,beforeUpdate:xe,updated:xe,beforeDestroy:xe,beforeUnmount:xe,destroyed:xe,unmounted:xe,activated:xe,deactivated:xe,errorCaptured:xe,serverPrefetch:xe,components:Os,directives:Os,watch:z_,provide:jl,inject:q_};function jl(t,e){return e?t?function(){return ke($(t)?t.call(this,this):t,$(e)?e.call(this,this):e)}:e:t}function q_(t,e){return Os(Ho(t),Ho(e))}function Ho(t){if(H(t)){const e={};for(let n=0;n<t.length;n++)e[t[n]]=t[n];return e}return t}function xe(t,e){return t?[...new Set([].concat(t,e))]:e}function Os(t,e){return t?ke(Object.create(null),t,e):e}function Gl(t,e){return t?H(t)&&H(e)?[...new Set([...t,...e])]:ke(Object.create(null),Bl(t),Bl(e??{})):e}function z_(t,e){if(!t)return e;if(!e)return t;const n=ke(Object.create(null),t);for(const s in e)n[s]=xe(t[s],e[s]);return n}function Dh(){return{app:null,config:{isNativeTag:Lp,performance:!1,globalProperties:{},optionMergeStrategies:{},errorHandler:void 0,warnHandler:void 0,compilerOptions:{}},mixins:[],components:{},directives:{},provides:Object.create(null),optionsCache:new WeakMap,propsCache:new WeakMap,emitsCache:new WeakMap}}let Y_=0;function Q_(t,e){return function(s,i=null){$(s)||(s=ke({},s)),i!=null&&!de(i)&&(i=null);const r=Dh(),o=new WeakSet,a=[];let l=!1;const c=r.app={_uid:Y_++,_component:s,_props:i,_container:null,_context:r,_instance:null,version:kg,get config(){return r.config},set config(h){},use(h,...u){return o.has(h)||(h&&$(h.install)?(o.add(h),h.install(c,...u)):$(h)&&(o.add(h),h(c,...u))),c},mixin(h){return r.mixins.includes(h)||r.mixins.push(h),c},component(h,u){return u?(r.components[h]=u,c):r.components[h]},directive(h,u){return u?(r.directives[h]=u,c):r.directives[h]},mount(h,u,d){if(!l){const _=c._ceVNode||Ee(s,i);return _.appContext=r,d===!0?d="svg":d===!1&&(d=void 0),t(_,h,d),l=!0,c._container=h,h.__vue_app__=c,Vr(_.component)}},onUnmount(h){a.push(h)},unmount(){l&&(wt(a,c._instance,16),t(null,c._container),delete c._container.__vue_app__)},provide(h,u){return r.provides[h]=u,c},runWithContext(h){const u=xn;xn=c;try{return h()}finally{xn=u}}};return c}}let xn=null;function Wi(t,e){if(Re){let n=Re.provides;const s=Re.parent&&Re.parent.provides;s===n&&(n=Re.provides=Object.create(s)),n[t]=e}}function et(t,e,n=!1){const s=Re||Be;if(s||xn){const i=xn?xn._context.provides:s?s.parent==null?s.vnode.appContext&&s.vnode.appContext.provides:s.parent.provides:void 0;if(i&&t in i)return i[t];if(arguments.length>1)return n&&$(e)?e.call(s&&s.proxy):e}}function J_(){return!!(Re||Be||xn)}const Mh={},Lh=()=>Object.create(Mh),Fh=t=>Object.getPrototypeOf(t)===Mh;function X_(t,e,n,s=!1){const i={},r=Lh();t.propsDefaults=Object.create(null),Uh(t,e,i,r);for(const o in t.propsOptions[0])o in i||(i[o]=void 0);n?t.props=s?i:_h(i):t.type.props?t.props=i:t.props=r,t.attrs=r}function Z_(t,e,n,s){const{props:i,attrs:r,vnode:{patchFlag:o}}=t,a=Z(i),[l]=t.propsOptions;let c=!1;if((s||o>0)&&!(o&16)){if(o&8){const h=t.vnode.dynamicProps;for(let u=0;u<h.length;u++){let d=h[u];if(Wr(t.emitsOptions,d))continue;const _=e[d];if(l)if(se(r,d))_!==r[d]&&(r[d]=_,c=!0);else{const m=tt(d);i[m]=Vo(l,a,m,_,t,!1)}else _!==r[d]&&(r[d]=_,c=!0)}}}else{Uh(t,e,i,r)&&(c=!0);let h;for(const u in a)(!e||!se(e,u)&&((h=Bn(u))===u||!se(e,h)))&&(l?n&&(n[u]!==void 0||n[h]!==void 0)&&(i[u]=Vo(l,a,u,void 0,t,!0)):delete i[u]);if(r!==a)for(const u in r)(!e||!se(e,u))&&(delete r[u],c=!0)}c&&At(t.attrs,"set","")}function Uh(t,e,n,s){const[i,r]=t.propsOptions;let o=!1,a;if(e)for(let l in e){if(Ms(l))continue;const c=e[l];let h;i&&se(i,h=tt(l))?!r||!r.includes(h)?n[h]=c:(a||(a={}))[h]=c:Wr(t.emitsOptions,l)||(!(l in s)||c!==s[l])&&(s[l]=c,o=!0)}if(r){const l=Z(n),c=a||ae;for(let h=0;h<r.length;h++){const u=r[h];n[u]=Vo(i,l,u,c[u],t,!se(c,u))}}return o}function Vo(t,e,n,s,i,r){const o=t[n];if(o!=null){const a=se(o,"default");if(a&&s===void 0){const l=o.default;if(o.type!==Function&&!o.skipFactory&&$(l)){const{propsDefaults:c}=i;if(n in c)s=c[n];else{const h=yi(i);s=c[n]=l.call(null,e),h()}}else s=l;i.ce&&i.ce._setProp(n,s)}o[0]&&(r&&!a?s=!1:o[1]&&(s===""||s===Bn(n))&&(s=!0))}return s}const eg=new WeakMap;function Wh(t,e,n=!1){const s=n?eg:e.propsCache,i=s.get(t);if(i)return i;const r=t.props,o={},a=[];let l=!1;if(!$(t)){const h=u=>{l=!0;const[d,_]=Wh(u,e,!0);ke(o,d),_&&a.push(..._)};!n&&e.mixins.length&&e.mixins.forEach(h),t.extends&&h(t.extends),t.mixins&&t.mixins.forEach(h)}if(!r&&!l)return de(t)&&s.set(t,Zn),Zn;if(H(r))for(let h=0;h<r.length;h++){const u=tt(r[h]);Kl(u)&&(o[u]=ae)}else if(r)for(const h in r){const u=tt(h);if(Kl(u)){const d=r[h],_=o[u]=H(d)||$(d)?{type:d}:ke({},d),m=_.type;let I=!1,O=!0;if(H(m))for(let D=0;D<m.length;++D){const k=m[D],M=$(k)&&k.name;if(M==="Boolean"){I=!0;break}else M==="String"&&(O=!1)}else I=$(m)&&m.name==="Boolean";_[0]=I,_[1]=O,(I||se(_,"default"))&&a.push(u)}}const c=[o,a];return de(t)&&s.set(t,c),c}function Kl(t){return t[0]!=="$"&&!Ms(t)}const Hh=t=>t[0]==="_"||t==="$stable",Oa=t=>H(t)?t.map(mt):[mt(t)],tg=(t,e,n)=>{if(e._n)return e;const s=Cn((...i)=>Oa(e(...i)),n);return s._c=!1,s},Vh=(t,e,n)=>{const s=t._ctx;for(const i in t){if(Hh(i))continue;const r=t[i];if($(r))e[i]=tg(i,r,s);else if(r!=null){const o=Oa(r);e[i]=()=>o}}},Bh=(t,e)=>{const n=Oa(e);t.slots.default=()=>n},$h=(t,e,n)=>{for(const s in e)(n||s!=="_")&&(t[s]=e[s])},ng=(t,e,n)=>{const s=t.slots=Lh();if(t.vnode.shapeFlag&32){const i=e._;i?($h(s,e,n),n&&zu(s,"_",i,!0)):Vh(e,s)}else e&&Bh(t,e)},sg=(t,e,n)=>{const{vnode:s,slots:i}=t;let r=!0,o=ae;if(s.shapeFlag&32){const a=e._;a?n&&a===1?r=!1:$h(i,e,n):(r=!e.$stable,Vh(e,i)),o=e}else e&&(Bh(t,e),o={default:1});if(r)for(const a in i)!Hh(a)&&o[a]==null&&delete i[a]},Ye=mg;function ig(t){return rg(t)}function rg(t,e){const n=Or();n.__VUE__=!0;const{insert:s,remove:i,patchProp:r,createElement:o,createText:a,createComment:l,setText:c,setElementText:h,parentNode:u,nextSibling:d,setScopeId:_=vt,insertStaticContent:m}=t,I=(f,p,g,v=null,w=null,E=null,R=void 0,T=null,S=!!p.dynamicChildren)=>{if(f===p)return;f&&!Ts(f,p)&&(v=y(f),He(f,w,E,!0),f=null),p.patchFlag===-2&&(S=!1,p.dynamicChildren=null);const{type:b,ref:U,shapeFlag:A}=p;switch(b){case Hr:O(f,p,g,v);break;case Mn:D(f,p,g,v);break;case Hi:f==null&&k(p,g,v,R);break;case Pt:Ke(f,p,g,v,w,E,R,T,S);break;default:A&1?j(f,p,g,v,w,E,R,T,S):A&6?We(f,p,g,v,w,E,R,T,S):(A&64||A&128)&&b.process(f,p,g,v,w,E,R,T,S,L)}U!=null&&w&&Zi(U,f&&f.ref,E,p||f,!p)},O=(f,p,g,v)=>{if(f==null)s(p.el=a(p.children),g,v);else{const w=p.el=f.el;p.children!==f.children&&c(w,p.children)}},D=(f,p,g,v)=>{f==null?s(p.el=l(p.children||""),g,v):p.el=f.el},k=(f,p,g,v)=>{[f.el,f.anchor]=m(f.children,p,g,v,f.el,f.anchor)},M=({el:f,anchor:p},g,v)=>{let w;for(;f&&f!==p;)w=d(f),s(f,g,v),f=w;s(p,g,v)},x=({el:f,anchor:p})=>{let g;for(;f&&f!==p;)g=d(f),i(f),f=g;i(p)},j=(f,p,g,v,w,E,R,T,S)=>{p.type==="svg"?R="svg":p.type==="math"&&(R="mathml"),f==null?ge(p,g,v,w,E,R,T,S):K(f,p,w,E,R,T,S)},ge=(f,p,g,v,w,E,R,T)=>{let S,b;const{props:U,shapeFlag:A,transition:F,dirs:B}=f;if(S=f.el=o(f.type,E,U&&U.is,U),A&8?h(S,f.children):A&16&&q(f.children,S,null,v,w,fo(f,E),R,T),B&&wn(f,null,v,"created"),X(S,f,f.scopeId,R,v),U){for(const ce in U)ce!=="value"&&!Ms(ce)&&r(S,ce,null,U[ce],E,v);"value"in U&&r(S,"value",null,U.value,E),(b=U.onVnodeBeforeMount)&&_t(b,v,f)}B&&wn(f,null,v,"beforeMount");const J=og(w,F);J&&F.beforeEnter(S),s(S,p,g),((b=U&&U.onVnodeMounted)||J||B)&&Ye(()=>{b&&_t(b,v,f),J&&F.enter(S),B&&wn(f,null,v,"mounted")},w)},X=(f,p,g,v,w)=>{if(g&&_(f,g),v)for(let E=0;E<v.length;E++)_(f,v[E]);if(w){let E=w.subTree;if(p===E||Yh(E.type)&&(E.ssContent===p||E.ssFallback===p)){const R=w.vnode;X(f,R,R.scopeId,R.slotScopeIds,w.parent)}}},q=(f,p,g,v,w,E,R,T,S=0)=>{for(let b=S;b<f.length;b++){const U=f[b]=T?Qt(f[b]):mt(f[b]);I(null,U,p,g,v,w,E,R,T)}},K=(f,p,g,v,w,E,R)=>{const T=p.el=f.el;let{patchFlag:S,dynamicChildren:b,dirs:U}=p;S|=f.patchFlag&16;const A=f.props||ae,F=p.props||ae;let B;if(g&&In(g,!1),(B=F.onVnodeBeforeUpdate)&&_t(B,g,p,f),U&&wn(p,f,g,"beforeUpdate"),g&&In(g,!0),(A.innerHTML&&F.innerHTML==null||A.textContent&&F.textContent==null)&&h(T,""),b?fe(f.dynamicChildren,b,T,g,v,fo(p,w),E):R||te(f,p,T,null,g,v,fo(p,w),E,!1),S>0){if(S&16)Oe(T,A,F,g,w);else if(S&2&&A.class!==F.class&&r(T,"class",null,F.class,w),S&4&&r(T,"style",A.style,F.style,w),S&8){const J=p.dynamicProps;for(let ce=0;ce<J.length;ce++){const ie=J[ce],qe=A[ie],Ve=F[ie];(Ve!==qe||ie==="value")&&r(T,ie,qe,Ve,w,g)}}S&1&&f.children!==p.children&&h(T,p.children)}else!R&&b==null&&Oe(T,A,F,g,w);((B=F.onVnodeUpdated)||U)&&Ye(()=>{B&&_t(B,g,p,f),U&&wn(p,f,g,"updated")},v)},fe=(f,p,g,v,w,E,R)=>{for(let T=0;T<p.length;T++){const S=f[T],b=p[T],U=S.el&&(S.type===Pt||!Ts(S,b)||S.shapeFlag&70)?u(S.el):g;I(S,b,U,null,v,w,E,R,!0)}},Oe=(f,p,g,v,w)=>{if(p!==g){if(p!==ae)for(const E in p)!Ms(E)&&!(E in g)&&r(f,E,p[E],null,w,v);for(const E in g){if(Ms(E))continue;const R=g[E],T=p[E];R!==T&&E!=="value"&&r(f,E,T,R,w,v)}"value"in g&&r(f,"value",p.value,g.value,w)}},Ke=(f,p,g,v,w,E,R,T,S)=>{const b=p.el=f?f.el:a(""),U=p.anchor=f?f.anchor:a("");let{patchFlag:A,dynamicChildren:F,slotScopeIds:B}=p;B&&(T=T?T.concat(B):B),f==null?(s(b,g,v),s(U,g,v),q(p.children||[],g,U,w,E,R,T,S)):A>0&&A&64&&F&&f.dynamicChildren?(fe(f.dynamicChildren,F,g,w,E,R,T),(p.key!=null||w&&p===w.subTree)&&jh(f,p,!0)):te(f,p,g,U,w,E,R,T,S)},We=(f,p,g,v,w,E,R,T,S)=>{p.slotScopeIds=T,f==null?p.shapeFlag&512?w.ctx.activate(p,g,v,R,S):En(p,g,v,w,E,R,S):$t(f,p,S)},En=(f,p,g,v,w,E,R)=>{const T=f.component=Cg(f,v,w);if(Th(f)&&(T.ctx.renderer=L),Sg(T,!1,R),T.asyncDep){if(w&&w.registerDep(T,_e,R),!f.el){const S=T.subTree=Ee(Mn);D(null,S,p,g)}}else _e(T,f,p,g,w,E,R)},$t=(f,p,g)=>{const v=p.component=f.component;if(_g(f,p,g))if(v.asyncDep&&!v.asyncResolved){G(v,p,g);return}else v.next=p,v.update();else p.el=f.el,v.vnode=p},_e=(f,p,g,v,w,E,R)=>{const T=()=>{if(f.isMounted){let{next:A,bu:F,u:B,parent:J,vnode:ce}=f;{const ft=Gh(f);if(ft){A&&(A.el=ce.el,G(f,A,R)),ft.asyncDep.then(()=>{f.isUnmounted||T()});return}}let ie=A,qe;In(f,!1),A?(A.el=ce.el,G(f,A,R)):A=ce,F&&Ui(F),(qe=A.props&&A.props.onVnodeBeforeUpdate)&&_t(qe,J,A,ce),In(f,!0);const Ve=zl(f),dt=f.subTree;f.subTree=Ve,I(dt,Ve,u(dt.el),y(dt),f,w,E),A.el=Ve.el,ie===null&&gg(f,Ve.el),B&&Ye(B,w),(qe=A.props&&A.props.onVnodeUpdated)&&Ye(()=>_t(qe,J,A,ce),w)}else{let A;const{el:F,props:B}=p,{bm:J,m:ce,parent:ie,root:qe,type:Ve}=f,dt=Us(p);In(f,!1),J&&Ui(J),!dt&&(A=B&&B.onVnodeBeforeMount)&&_t(A,ie,p),In(f,!0);{qe.ce&&qe.ce._injectChildStyle(Ve);const ft=f.subTree=zl(f);I(null,ft,g,v,f,w,E),p.el=ft.el}if(ce&&Ye(ce,w),!dt&&(A=B&&B.onVnodeMounted)){const ft=p;Ye(()=>_t(A,ie,ft),w)}(p.shapeFlag&256||ie&&Us(ie.vnode)&&ie.vnode.shapeFlag&256)&&f.a&&Ye(f.a,w),f.isMounted=!0,p=g=v=null}};f.scope.on();const S=f.effect=new th(T);f.scope.off();const b=f.update=S.run.bind(S),U=f.job=S.runIfDirty.bind(S);U.i=f,U.id=f.uid,S.scheduler=()=>Na(U),In(f,!0),b()},G=(f,p,g)=>{p.component=f;const v=f.vnode.props;f.vnode=p,f.next=null,Z_(f,p.props,v,g),sg(f,p.children,g),pn(),Wl(f),_n()},te=(f,p,g,v,w,E,R,T,S=!1)=>{const b=f&&f.children,U=f?f.shapeFlag:0,A=p.children,{patchFlag:F,shapeFlag:B}=p;if(F>0){if(F&128){jt(b,A,g,v,w,E,R,T,S);return}else if(F&256){bt(b,A,g,v,w,E,R,T,S);return}}B&8?(U&16&&Je(b,w,E),A!==b&&h(g,A)):U&16?B&16?jt(b,A,g,v,w,E,R,T,S):Je(b,w,E,!0):(U&8&&h(g,""),B&16&&q(A,g,v,w,E,R,T,S))},bt=(f,p,g,v,w,E,R,T,S)=>{f=f||Zn,p=p||Zn;const b=f.length,U=p.length,A=Math.min(b,U);let F;for(F=0;F<A;F++){const B=p[F]=S?Qt(p[F]):mt(p[F]);I(f[F],B,g,null,w,E,R,T,S)}b>U?Je(f,w,E,!0,!1,A):q(p,g,v,w,E,R,T,S,A)},jt=(f,p,g,v,w,E,R,T,S)=>{let b=0;const U=p.length;let A=f.length-1,F=U-1;for(;b<=A&&b<=F;){const B=f[b],J=p[b]=S?Qt(p[b]):mt(p[b]);if(Ts(B,J))I(B,J,g,null,w,E,R,T,S);else break;b++}for(;b<=A&&b<=F;){const B=f[A],J=p[F]=S?Qt(p[F]):mt(p[F]);if(Ts(B,J))I(B,J,g,null,w,E,R,T,S);else break;A--,F--}if(b>A){if(b<=F){const B=F+1,J=B<U?p[B].el:v;for(;b<=F;)I(null,p[b]=S?Qt(p[b]):mt(p[b]),g,J,w,E,R,T,S),b++}}else if(b>F)for(;b<=A;)He(f[b],w,E,!0),b++;else{const B=b,J=b,ce=new Map;for(b=J;b<=F;b++){const ze=p[b]=S?Qt(p[b]):mt(p[b]);ze.key!=null&&ce.set(ze.key,b)}let ie,qe=0;const Ve=F-J+1;let dt=!1,ft=0;const Cs=new Array(Ve);for(b=0;b<Ve;b++)Cs[b]=0;for(b=B;b<=A;b++){const ze=f[b];if(qe>=Ve){He(ze,w,E,!0);continue}let pt;if(ze.key!=null)pt=ce.get(ze.key);else for(ie=J;ie<=F;ie++)if(Cs[ie-J]===0&&Ts(ze,p[ie])){pt=ie;break}pt===void 0?He(ze,w,E,!0):(Cs[pt-J]=b+1,pt>=ft?ft=pt:dt=!0,I(ze,p[pt],g,null,w,E,R,T,S),qe++)}const xl=dt?ag(Cs):Zn;for(ie=xl.length-1,b=Ve-1;b>=0;b--){const ze=J+b,pt=p[ze],Dl=ze+1<U?p[ze+1].el:v;Cs[b]===0?I(null,pt,g,Dl,w,E,R,T,S):dt&&(ie<0||b!==xl[ie]?ht(pt,g,Dl,2):ie--)}}},ht=(f,p,g,v,w=null)=>{const{el:E,type:R,transition:T,children:S,shapeFlag:b}=f;if(b&6){ht(f.component.subTree,p,g,v);return}if(b&128){f.suspense.move(p,g,v);return}if(b&64){R.move(f,p,g,L);return}if(R===Pt){s(E,p,g);for(let A=0;A<S.length;A++)ht(S[A],p,g,v);s(f.anchor,p,g);return}if(R===Hi){M(f,p,g);return}if(v!==2&&b&1&&T)if(v===0)T.beforeEnter(E),s(E,p,g),Ye(()=>T.enter(E),w);else{const{leave:A,delayLeave:F,afterLeave:B}=T,J=()=>s(E,p,g),ce=()=>{A(E,()=>{J(),B&&B()})};F?F(E,J,ce):ce()}else s(E,p,g)},He=(f,p,g,v=!1,w=!1)=>{const{type:E,props:R,ref:T,children:S,dynamicChildren:b,shapeFlag:U,patchFlag:A,dirs:F,cacheIndex:B}=f;if(A===-2&&(w=!1),T!=null&&Zi(T,null,g,f,!0),B!=null&&(p.renderCache[B]=void 0),U&256){p.ctx.deactivate(f);return}const J=U&1&&F,ce=!Us(f);let ie;if(ce&&(ie=R&&R.onVnodeBeforeUnmount)&&_t(ie,p,f),U&6)ki(f.component,g,v);else{if(U&128){f.suspense.unmount(g,v);return}J&&wn(f,null,p,"beforeUnmount"),U&64?f.type.remove(f,p,g,L,v):b&&!b.hasOnce&&(E!==Pt||A>0&&A&64)?Je(b,p,g,!1,!0):(E===Pt&&A&384||!w&&U&16)&&Je(S,p,g),v&&jn(f)}(ce&&(ie=R&&R.onVnodeUnmounted)||J)&&Ye(()=>{ie&&_t(ie,p,f),J&&wn(f,null,p,"unmounted")},g)},jn=f=>{const{type:p,el:g,anchor:v,transition:w}=f;if(p===Pt){Gn(g,v);return}if(p===Hi){x(f);return}const E=()=>{i(g),w&&!w.persisted&&w.afterLeave&&w.afterLeave()};if(f.shapeFlag&1&&w&&!w.persisted){const{leave:R,delayLeave:T}=w,S=()=>R(g,E);T?T(f.el,E,S):S()}else E()},Gn=(f,p)=>{let g;for(;f!==p;)g=d(f),i(f),f=g;i(p)},ki=(f,p,g)=>{const{bum:v,scope:w,job:E,subTree:R,um:T,m:S,a:b}=f;ql(S),ql(b),v&&Ui(v),w.stop(),E&&(E.flags|=8,He(R,f,p,g)),T&&Ye(T,p),Ye(()=>{f.isUnmounted=!0},p),p&&p.pendingBranch&&!p.isUnmounted&&f.asyncDep&&!f.asyncResolved&&f.suspenseId===p.pendingId&&(p.deps--,p.deps===0&&p.resolve())},Je=(f,p,g,v=!1,w=!1,E=0)=>{for(let R=E;R<f.length;R++)He(f[R],p,g,v,w)},y=f=>{if(f.shapeFlag&6)return y(f.component.subTree);if(f.shapeFlag&128)return f.suspense.next();const p=d(f.anchor||f.el),g=p&&p[A_];return g?d(g):p};let N=!1;const P=(f,p,g)=>{f==null?p._vnode&&He(p._vnode,null,null,!0):I(p._vnode||null,f,p,null,null,null,g),p._vnode=f,N||(N=!0,Wl(),wh(),N=!1)},L={p:I,um:He,m:ht,r:jn,mt:En,mc:q,pc:te,pbc:fe,n:y,o:t};return{render:P,hydrate:void 0,createApp:Q_(P)}}function fo({type:t,props:e},n){return n==="svg"&&t==="foreignObject"||n==="mathml"&&t==="annotation-xml"&&e&&e.encoding&&e.encoding.includes("html")?void 0:n}function In({effect:t,job:e},n){n?(t.flags|=32,e.flags|=4):(t.flags&=-33,e.flags&=-5)}function og(t,e){return(!t||t&&!t.pendingBranch)&&e&&!e.persisted}function jh(t,e,n=!1){const s=t.children,i=e.children;if(H(s)&&H(i))for(let r=0;r<s.length;r++){const o=s[r];let a=i[r];a.shapeFlag&1&&!a.dynamicChildren&&((a.patchFlag<=0||a.patchFlag===32)&&(a=i[r]=Qt(i[r]),a.el=o.el),!n&&a.patchFlag!==-2&&jh(o,a)),a.type===Hr&&(a.el=o.el)}}function ag(t){const e=t.slice(),n=[0];let s,i,r,o,a;const l=t.length;for(s=0;s<l;s++){const c=t[s];if(c!==0){if(i=n[n.length-1],t[i]<c){e[s]=i,n.push(s);continue}for(r=0,o=n.length-1;r<o;)a=r+o>>1,t[n[a]]<c?r=a+1:o=a;c<t[n[r]]&&(r>0&&(e[s]=n[r-1]),n[r]=s)}}for(r=n.length,o=n[r-1];r-- >0;)n[r]=o,o=e[o];return n}function Gh(t){const e=t.subTree.component;if(e)return e.asyncDep&&!e.asyncResolved?e:Gh(e)}function ql(t){if(t)for(let e=0;e<t.length;e++)t[e].flags|=8}const lg=Symbol.for("v-scx"),cg=()=>et(lg);function ns(t,e,n){return Kh(t,e,n)}function Kh(t,e,n=ae){const{immediate:s,deep:i,flush:r,once:o}=n,a=ke({},n),l=e&&s||!e&&r!=="post";let c;if(ei){if(r==="sync"){const _=cg();c=_.__watcherHandles||(_.__watcherHandles=[])}else if(!l){const _=()=>{};return _.stop=vt,_.resume=vt,_.pause=vt,_}}const h=Re;a.call=(_,m,I)=>wt(_,h,m,I);let u=!1;r==="post"?a.scheduler=_=>{Ye(_,h&&h.suspense)}:r!=="sync"&&(u=!0,a.scheduler=(_,m)=>{m?_():Na(_)}),a.augmentJob=_=>{e&&(_.flags|=4),u&&(_.flags|=2,h&&(_.id=h.uid,_.i=h))};const d=S_(t,e,a);return ei&&(c?c.push(d):l&&d()),d}function ug(t,e,n){const s=this.proxy,i=ye(t)?t.includes(".")?qh(s,t):()=>s[t]:t.bind(s,s);let r;$(e)?r=e:(r=e.handler,n=e);const o=yi(this),a=Kh(i,r.bind(s),n);return o(),a}function qh(t,e){const n=e.split(".");return()=>{let s=t;for(let i=0;i<n.length&&s;i++)s=s[n[i]];return s}}const hg=(t,e)=>e==="modelValue"||e==="model-value"?t.modelModifiers:t[`${e}Modifiers`]||t[`${tt(e)}Modifiers`]||t[`${Bn(e)}Modifiers`];function dg(t,e,...n){if(t.isUnmounted)return;const s=t.vnode.props||ae;let i=n;const r=e.startsWith("update:"),o=r&&hg(s,e.slice(7));o&&(o.trim&&(i=n.map(h=>ye(h)?h.trim():h)),o.number&&(i=n.map(zi)));let a,l=s[a=oo(e)]||s[a=oo(tt(e))];!l&&r&&(l=s[a=oo(Bn(e))]),l&&wt(l,t,6,i);const c=s[a+"Once"];if(c){if(!t.emitted)t.emitted={};else if(t.emitted[a])return;t.emitted[a]=!0,wt(c,t,6,i)}}function zh(t,e,n=!1){const s=e.emitsCache,i=s.get(t);if(i!==void 0)return i;const r=t.emits;let o={},a=!1;if(!$(t)){const l=c=>{const h=zh(c,e,!0);h&&(a=!0,ke(o,h))};!n&&e.mixins.length&&e.mixins.forEach(l),t.extends&&l(t.extends),t.mixins&&t.mixins.forEach(l)}return!r&&!a?(de(t)&&s.set(t,null),null):(H(r)?r.forEach(l=>o[l]=null):ke(o,r),de(t)&&s.set(t,o),o)}function Wr(t,e){return!t||!Pr(e)?!1:(e=e.slice(2).replace(/Once$/,""),se(t,e[0].toLowerCase()+e.slice(1))||se(t,Bn(e))||se(t,e))}function zl(t){const{type:e,vnode:n,proxy:s,withProxy:i,propsOptions:[r],slots:o,attrs:a,emit:l,render:c,renderCache:h,props:u,data:d,setupState:_,ctx:m,inheritAttrs:I}=t,O=Xi(t);let D,k;try{if(n.shapeFlag&4){const x=i||s,j=x;D=mt(c.call(j,x,h,u,_,d,m)),k=a}else{const x=e;D=mt(x.length>1?x(u,{attrs:a,slots:o,emit:l}):x(u,null)),k=e.props?a:fg(a)}}catch(x){Hs.length=0,Lr(x,t,1),D=Ee(Mn)}let M=D;if(k&&I!==!1){const x=Object.keys(k),{shapeFlag:j}=M;x.length&&j&7&&(r&&x.some(ma)&&(k=pg(k,r)),M=cs(M,k,!1,!0))}return n.dirs&&(M=cs(M,null,!1,!0),M.dirs=M.dirs?M.dirs.concat(n.dirs):n.dirs),n.transition&&ka(M,n.transition),D=M,Xi(O),D}const fg=t=>{let e;for(const n in t)(n==="class"||n==="style"||Pr(n))&&((e||(e={}))[n]=t[n]);return e},pg=(t,e)=>{const n={};for(const s in t)(!ma(s)||!(s.slice(9)in e))&&(n[s]=t[s]);return n};function _g(t,e,n){const{props:s,children:i,component:r}=t,{props:o,children:a,patchFlag:l}=e,c=r.emitsOptions;if(e.dirs||e.transition)return!0;if(n&&l>=0){if(l&1024)return!0;if(l&16)return s?Yl(s,o,c):!!o;if(l&8){const h=e.dynamicProps;for(let u=0;u<h.length;u++){const d=h[u];if(o[d]!==s[d]&&!Wr(c,d))return!0}}}else return(i||a)&&(!a||!a.$stable)?!0:s===o?!1:s?o?Yl(s,o,c):!0:!!o;return!1}function Yl(t,e,n){const s=Object.keys(e);if(s.length!==Object.keys(t).length)return!0;for(let i=0;i<s.length;i++){const r=s[i];if(e[r]!==t[r]&&!Wr(n,r))return!0}return!1}function gg({vnode:t,parent:e},n){for(;e;){const s=e.subTree;if(s.suspense&&s.suspense.activeBranch===t&&(s.el=t.el),s===t)(t=e.vnode).el=n,e=e.parent;else break}}const Yh=t=>t.__isSuspense;function mg(t,e){e&&e.pendingBranch?H(t)?e.effects.push(...t):e.effects.push(t):P_(t)}const Pt=Symbol.for("v-fgt"),Hr=Symbol.for("v-txt"),Mn=Symbol.for("v-cmt"),Hi=Symbol.for("v-stc"),Hs=[];let Qe=null;function qt(t=!1){Hs.push(Qe=t?null:[])}function vg(){Hs.pop(),Qe=Hs[Hs.length-1]||null}let Zs=1;function Ql(t,e=!1){Zs+=t,t<0&&Qe&&e&&(Qe.hasOnce=!0)}function Qh(t){return t.dynamicChildren=Zs>0?Qe||Zn:null,vg(),Zs>0&&Qe&&Qe.push(t),t}function qn(t,e,n,s,i,r){return Qh(be(t,e,n,s,i,r,!0))}function Jh(t,e,n,s,i){return Qh(Ee(t,e,n,s,i,!0))}function tr(t){return t?t.__v_isVNode===!0:!1}function Ts(t,e){return t.type===e.type&&t.key===e.key}const Xh=({key:t})=>t??null,Vi=({ref:t,ref_key:e,ref_for:n})=>(typeof t=="number"&&(t=""+t),t!=null?ye(t)||ve(t)||$(t)?{i:Be,r:t,k:e,f:!!n}:t:null);function be(t,e=null,n=null,s=0,i=null,r=t===Pt?0:1,o=!1,a=!1){const l={__v_isVNode:!0,__v_skip:!0,type:t,props:e,key:e&&Xh(e),ref:e&&Vi(e),scopeId:bh,slotScopeIds:null,children:n,component:null,suspense:null,ssContent:null,ssFallback:null,dirs:null,transition:null,el:null,anchor:null,target:null,targetStart:null,targetAnchor:null,staticCount:0,shapeFlag:r,patchFlag:s,dynamicProps:i,dynamicChildren:null,appContext:null,ctx:Be};return a?(xa(l,n),r&128&&t.normalize(l)):n&&(l.shapeFlag|=ye(n)?8:16),Zs>0&&!o&&Qe&&(l.patchFlag>0||r&6)&&l.patchFlag!==32&&Qe.push(l),l}const Ee=yg;function yg(t,e=null,n=null,s=0,i=null,r=!1){if((!t||t===Nh)&&(t=Mn),tr(t)){const a=cs(t,e,!0);return n&&xa(a,n),Zs>0&&!r&&Qe&&(a.shapeFlag&6?Qe[Qe.indexOf(t)]=a:Qe.push(a)),a.patchFlag=-2,a}if(Ng(t)&&(t=t.__vccOpts),e){e=Eg(e);let{class:a,style:l}=e;a&&!ye(a)&&(e.class=wa(a)),de(l)&&(Pa(l)&&!H(l)&&(l=ke({},l)),e.style=Ea(l))}const o=ye(t)?1:Yh(t)?128:N_(t)?64:de(t)?4:$(t)?2:0;return be(t,e,n,s,i,o,r,!0)}function Eg(t){return t?Pa(t)||Fh(t)?ke({},t):t:null}function cs(t,e,n=!1,s=!1){const{props:i,ref:r,patchFlag:o,children:a,transition:l}=t,c=e?wg(i||{},e):i,h={__v_isVNode:!0,__v_skip:!0,type:t.type,props:c,key:c&&Xh(c),ref:e&&e.ref?n&&r?H(r)?r.concat(Vi(e)):[r,Vi(e)]:Vi(e):r,scopeId:t.scopeId,slotScopeIds:t.slotScopeIds,children:a,target:t.target,targetStart:t.targetStart,targetAnchor:t.targetAnchor,staticCount:t.staticCount,shapeFlag:t.shapeFlag,patchFlag:e&&t.type!==Pt?o===-1?16:o|16:o,dynamicProps:t.dynamicProps,dynamicChildren:t.dynamicChildren,appContext:t.appContext,dirs:t.dirs,transition:l,component:t.component,suspense:t.suspense,ssContent:t.ssContent&&cs(t.ssContent),ssFallback:t.ssFallback&&cs(t.ssFallback),el:t.el,anchor:t.anchor,ctx:t.ctx,ce:t.ce};return l&&s&&ka(h,l.clone(h)),h}function Sn(t=" ",e=0){return Ee(Hr,null,t,e)}function US(t,e){const n=Ee(Hi,null,t);return n.staticCount=e,n}function Jl(t="",e=!1){return e?(qt(),Jh(Mn,null,t)):Ee(Mn,null,t)}function mt(t){return t==null||typeof t=="boolean"?Ee(Mn):H(t)?Ee(Pt,null,t.slice()):tr(t)?Qt(t):Ee(Hr,null,String(t))}function Qt(t){return t.el===null&&t.patchFlag!==-1||t.memo?t:cs(t)}function xa(t,e){let n=0;const{shapeFlag:s}=t;if(e==null)e=null;else if(H(e))n=16;else if(typeof e=="object")if(s&65){const i=e.default;i&&(i._c&&(i._d=!1),xa(t,i()),i._c&&(i._d=!0));return}else{n=32;const i=e._;!i&&!Fh(e)?e._ctx=Be:i===3&&Be&&(Be.slots._===1?e._=1:(e._=2,t.patchFlag|=1024))}else $(e)?(e={default:e,_ctx:Be},n=32):(e=String(e),s&64?(n=16,e=[Sn(e)]):n=8);t.children=e,t.shapeFlag|=n}function wg(...t){const e={};for(let n=0;n<t.length;n++){const s=t[n];for(const i in s)if(i==="class")e.class!==s.class&&(e.class=wa([e.class,s.class]));else if(i==="style")e.style=Ea([e.style,s.style]);else if(Pr(i)){const r=e[i],o=s[i];o&&r!==o&&!(H(r)&&r.includes(o))&&(e[i]=r?[].concat(r,o):o)}else i!==""&&(e[i]=s[i])}return e}function _t(t,e,n,s=null){wt(t,e,7,[n,s])}const Ig=Dh();let bg=0;function Cg(t,e,n){const s=t.type,i=(e?e.appContext:t.appContext)||Ig,r={uid:bg++,vnode:t,type:s,parent:e,appContext:i,root:null,next:null,subTree:null,effect:null,update:null,job:null,scope:new Xu(!0),render:null,proxy:null,exposed:null,exposeProxy:null,withProxy:null,provides:e?e.provides:Object.create(i.provides),ids:e?e.ids:["",0,0],accessCache:null,renderCache:[],components:null,directives:null,propsOptions:Wh(s,i),emitsOptions:zh(s,i),emit:null,emitted:null,propsDefaults:ae,inheritAttrs:s.inheritAttrs,ctx:ae,data:ae,props:ae,attrs:ae,slots:ae,refs:ae,setupState:ae,setupContext:null,suspense:n,suspenseId:n?n.pendingId:0,asyncDep:null,asyncResolved:!1,isMounted:!1,isUnmounted:!1,isDeactivated:!1,bc:null,c:null,bm:null,m:null,bu:null,u:null,um:null,bum:null,da:null,a:null,rtg:null,rtc:null,ec:null,sp:null};return r.ctx={_:r},r.root=e?e.root:r,r.emit=dg.bind(null,r),t.ce&&t.ce(r),r}let Re=null,nr,Bo;{const t=Or(),e=(n,s)=>{let i;return(i=t[n])||(i=t[n]=[]),i.push(s),r=>{i.length>1?i.forEach(o=>o(r)):i[0](r)}};nr=e("__VUE_INSTANCE_SETTERS__",n=>Re=n),Bo=e("__VUE_SSR_SETTERS__",n=>ei=n)}const yi=t=>{const e=Re;return nr(t),t.scope.on(),()=>{t.scope.off(),nr(e)}},Xl=()=>{Re&&Re.scope.off(),nr(null)};function Zh(t){return t.vnode.shapeFlag&4}let ei=!1;function Sg(t,e=!1,n=!1){e&&Bo(e);const{props:s,children:i}=t.vnode,r=Zh(t);X_(t,s,r,e),ng(t,i,n);const o=r?Tg(t,e):void 0;return e&&Bo(!1),o}function Tg(t,e){const n=t.type;t.accessCache=Object.create(null),t.proxy=new Proxy(t.ctx,$_);const{setup:s}=n;if(s){pn();const i=t.setupContext=s.length>1?Pg(t):null,r=yi(t),o=vi(s,t,0,[t.props,i]),a=Gu(o);if(_n(),r(),(a||t.sp)&&!Us(t)&&Sh(t),a){if(o.then(Xl,Xl),e)return o.then(l=>{Zl(t,l)}).catch(l=>{Lr(l,t,0)});t.asyncDep=o}else Zl(t,o)}else ed(t)}function Zl(t,e,n){$(e)?t.type.__ssrInlineRender?t.ssrRender=e:t.render=e:de(e)&&(t.setupState=vh(e)),ed(t)}function ed(t,e,n){const s=t.type;t.render||(t.render=s.render||vt);{const i=yi(t);pn();try{j_(t)}finally{_n(),i()}}}const Rg={get(t,e){return Pe(t,"get",""),t[e]}};function Pg(t){const e=n=>{t.exposed=n||{}};return{attrs:new Proxy(t.attrs,Rg),slots:t.slots,emit:t.emit,expose:e}}function Vr(t){return t.exposed?t.exposeProxy||(t.exposeProxy=new Proxy(vh(Aa(t.exposed)),{get(e,n){if(n in e)return e[n];if(n in Ws)return Ws[n](t)},has(e,n){return n in e||n in Ws}})):t.proxy}function Ag(t,e=!0){return $(t)?t.displayName||t.name:t.name||e&&t.__name}function Ng(t){return $(t)&&"__vccOpts"in t}const Xe=(t,e)=>b_(t,e,ei);function td(t,e,n){const s=arguments.length;return s===2?de(e)&&!H(e)?tr(e)?Ee(t,null,[e]):Ee(t,e):Ee(t,null,e):(s>3?n=Array.prototype.slice.call(arguments,2):s===3&&tr(n)&&(n=[n]),Ee(t,e,n))}const kg="3.5.13";/**
* @vue/runtime-dom v3.5.13
* (c) 2018-present Yuxi (Evan) You and Vue contributors
* @license MIT
**/let $o;const ec=typeof window<"u"&&window.trustedTypes;if(ec)try{$o=ec.createPolicy("vue",{createHTML:t=>t})}catch{}const nd=$o?t=>$o.createHTML(t):t=>t,Og="http://www.w3.org/2000/svg",xg="http://www.w3.org/1998/Math/MathML",Rt=typeof document<"u"?document:null,tc=Rt&&Rt.createElement("template"),Dg={insert:(t,e,n)=>{e.insertBefore(t,n||null)},remove:t=>{const e=t.parentNode;e&&e.removeChild(t)},createElement:(t,e,n,s)=>{const i=e==="svg"?Rt.createElementNS(Og,t):e==="mathml"?Rt.createElementNS(xg,t):n?Rt.createElement(t,{is:n}):Rt.createElement(t);return t==="select"&&s&&s.multiple!=null&&i.setAttribute("multiple",s.multiple),i},createText:t=>Rt.createTextNode(t),createComment:t=>Rt.createComment(t),setText:(t,e)=>{t.nodeValue=e},setElementText:(t,e)=>{t.textContent=e},parentNode:t=>t.parentNode,nextSibling:t=>t.nextSibling,querySelector:t=>Rt.querySelector(t),setScopeId(t,e){t.setAttribute(e,"")},insertStaticContent(t,e,n,s,i,r){const o=n?n.previousSibling:e.lastChild;if(i&&(i===r||i.nextSibling))for(;e.insertBefore(i.cloneNode(!0),n),!(i===r||!(i=i.nextSibling)););else{tc.innerHTML=nd(s==="svg"?`<svg>${t}</svg>`:s==="mathml"?`<math>${t}</math>`:t);const a=tc.content;if(s==="svg"||s==="mathml"){const l=a.firstChild;for(;l.firstChild;)a.appendChild(l.firstChild);a.removeChild(l)}e.insertBefore(a,n)}return[o?o.nextSibling:e.firstChild,n?n.previousSibling:e.lastChild]}},Mg=Symbol("_vtc");function Lg(t,e,n){const s=t[Mg];s&&(e=(e?[e,...s]:[...s]).join(" ")),e==null?t.removeAttribute("class"):n?t.setAttribute("class",e):t.className=e}const nc=Symbol("_vod"),Fg=Symbol("_vsh"),Ug=Symbol(""),Wg=/(^|;)\s*display\s*:/;function Hg(t,e,n){const s=t.style,i=ye(n);let r=!1;if(n&&!i){if(e)if(ye(e))for(const o of e.split(";")){const a=o.slice(0,o.indexOf(":")).trim();n[a]==null&&Bi(s,a,"")}else for(const o in e)n[o]==null&&Bi(s,o,"");for(const o in n)o==="display"&&(r=!0),Bi(s,o,n[o])}else if(i){if(e!==n){const o=s[Ug];o&&(n+=";"+o),s.cssText=n,r=Wg.test(n)}}else e&&t.removeAttribute("style");nc in t&&(t[nc]=r?s.display:"",t[Fg]&&(s.display="none"))}const sc=/\s*!important$/;function Bi(t,e,n){if(H(n))n.forEach(s=>Bi(t,e,s));else if(n==null&&(n=""),e.startsWith("--"))t.setProperty(e,n);else{const s=Vg(t,e);sc.test(n)?t.setProperty(Bn(s),n.replace(sc,""),"important"):t[s]=n}}const ic=["Webkit","Moz","ms"],po={};function Vg(t,e){const n=po[e];if(n)return n;let s=tt(e);if(s!=="filter"&&s in t)return po[e]=s;s=kr(s);for(let i=0;i<ic.length;i++){const r=ic[i]+s;if(r in t)return po[e]=r}return e}const rc="http://www.w3.org/1999/xlink";function oc(t,e,n,s,i,r=Kp(e)){s&&e.startsWith("xlink:")?n==null?t.removeAttributeNS(rc,e.slice(6,e.length)):t.setAttributeNS(rc,e,n):n==null||r&&!Yu(n)?t.removeAttribute(e):t.setAttribute(e,r?"":Et(n)?String(n):n)}function ac(t,e,n,s,i){if(e==="innerHTML"||e==="textContent"){n!=null&&(t[e]=e==="innerHTML"?nd(n):n);return}const r=t.tagName;if(e==="value"&&r!=="PROGRESS"&&!r.includes("-")){const a=r==="OPTION"?t.getAttribute("value")||"":t.value,l=n==null?t.type==="checkbox"?"on":"":String(n);(a!==l||!("_value"in t))&&(t.value=l),n==null&&t.removeAttribute(e),t._value=n;return}let o=!1;if(n===""||n==null){const a=typeof t[e];a==="boolean"?n=Yu(n):n==null&&a==="string"?(n="",o=!0):a==="number"&&(n=0,o=!0)}try{t[e]=n}catch{}o&&t.removeAttribute(i||e)}function Pn(t,e,n,s){t.addEventListener(e,n,s)}function Bg(t,e,n,s){t.removeEventListener(e,n,s)}const lc=Symbol("_vei");function $g(t,e,n,s,i=null){const r=t[lc]||(t[lc]={}),o=r[e];if(s&&o)o.value=s;else{const[a,l]=jg(e);if(s){const c=r[e]=qg(s,i);Pn(t,a,c,l)}else o&&(Bg(t,a,o,l),r[e]=void 0)}}const cc=/(?:Once|Passive|Capture)$/;function jg(t){let e;if(cc.test(t)){e={};let s;for(;s=t.match(cc);)t=t.slice(0,t.length-s[0].length),e[s[0].toLowerCase()]=!0}return[t[2]===":"?t.slice(3):Bn(t.slice(2)),e]}let _o=0;const Gg=Promise.resolve(),Kg=()=>_o||(Gg.then(()=>_o=0),_o=Date.now());function qg(t,e){const n=s=>{if(!s._vts)s._vts=Date.now();else if(s._vts<=n.attached)return;wt(zg(s,n.value),e,5,[s])};return n.value=t,n.attached=Kg(),n}function zg(t,e){if(H(e)){const n=t.stopImmediatePropagation;return t.stopImmediatePropagation=()=>{n.call(t),t._stopped=!0},e.map(s=>i=>!i._stopped&&s&&s(i))}else return e}const uc=t=>t.charCodeAt(0)===111&&t.charCodeAt(1)===110&&t.charCodeAt(2)>96&&t.charCodeAt(2)<123,Yg=(t,e,n,s,i,r)=>{const o=i==="svg";e==="class"?Lg(t,s,o):e==="style"?Hg(t,n,s):Pr(e)?ma(e)||$g(t,e,n,s,r):(e[0]==="."?(e=e.slice(1),!0):e[0]==="^"?(e=e.slice(1),!1):Qg(t,e,s,o))?(ac(t,e,s),!t.tagName.includes("-")&&(e==="value"||e==="checked"||e==="selected")&&oc(t,e,s,o,r,e!=="value")):t._isVueCE&&(/[A-Z]/.test(e)||!ye(s))?ac(t,tt(e),s,r,e):(e==="true-value"?t._trueValue=s:e==="false-value"&&(t._falseValue=s),oc(t,e,s,o))};function Qg(t,e,n,s){if(s)return!!(e==="innerHTML"||e==="textContent"||e in t&&uc(e)&&$(n));if(e==="spellcheck"||e==="draggable"||e==="translate"||e==="form"||e==="list"&&t.tagName==="INPUT"||e==="type"&&t.tagName==="TEXTAREA")return!1;if(e==="width"||e==="height"){const i=t.tagName;if(i==="IMG"||i==="VIDEO"||i==="CANVAS"||i==="SOURCE")return!1}return uc(e)&&ye(n)?!1:e in t}const sr=t=>{const e=t.props["onUpdate:modelValue"]||!1;return H(e)?n=>Ui(e,n):e};function Jg(t){t.target.composing=!0}function hc(t){const e=t.target;e.composing&&(e.composing=!1,e.dispatchEvent(new Event("input")))}const ss=Symbol("_assign"),WS={created(t,{modifiers:{lazy:e,trim:n,number:s}},i){t[ss]=sr(i);const r=s||i.props&&i.props.type==="number";Pn(t,e?"change":"input",o=>{if(o.target.composing)return;let a=t.value;n&&(a=a.trim()),r&&(a=zi(a)),t[ss](a)}),n&&Pn(t,"change",()=>{t.value=t.value.trim()}),e||(Pn(t,"compositionstart",Jg),Pn(t,"compositionend",hc),Pn(t,"change",hc))},mounted(t,{value:e}){t.value=e??""},beforeUpdate(t,{value:e,oldValue:n,modifiers:{lazy:s,trim:i,number:r}},o){if(t[ss]=sr(o),t.composing)return;const a=(r||t.type==="number")&&!/^0\d/.test(t.value)?zi(t.value):t.value,l=e??"";a!==l&&(document.activeElement===t&&t.type!=="range"&&(s&&e===n||i&&t.value.trim()===l)||(t.value=l))}},HS={deep:!0,created(t,{value:e,modifiers:{number:n}},s){const i=Ar(e);Pn(t,"change",()=>{const r=Array.prototype.filter.call(t.options,o=>o.selected).map(o=>n?zi(ir(o)):ir(o));t[ss](t.multiple?i?new Set(r):r:r[0]),t._assigning=!0,Fr(()=>{t._assigning=!1})}),t[ss]=sr(s)},mounted(t,{value:e}){dc(t,e)},beforeUpdate(t,e,n){t[ss]=sr(n)},updated(t,{value:e}){t._assigning||dc(t,e)}};function dc(t,e){const n=t.multiple,s=H(e);if(!(n&&!s&&!Ar(e))){for(let i=0,r=t.options.length;i<r;i++){const o=t.options[i],a=ir(o);if(n)if(s){const l=typeof a;l==="string"||l==="number"?o.selected=e.some(c=>String(c)===String(a)):o.selected=zp(e,a)>-1}else o.selected=e.has(a);else if(xr(ir(o),e)){t.selectedIndex!==i&&(t.selectedIndex=i);return}}!n&&t.selectedIndex!==-1&&(t.selectedIndex=-1)}}function ir(t){return"_value"in t?t._value:t.value}const Xg=["ctrl","shift","alt","meta"],Zg={stop:t=>t.stopPropagation(),prevent:t=>t.preventDefault(),self:t=>t.target!==t.currentTarget,ctrl:t=>!t.ctrlKey,shift:t=>!t.shiftKey,alt:t=>!t.altKey,meta:t=>!t.metaKey,left:t=>"button"in t&&t.button!==0,middle:t=>"button"in t&&t.button!==1,right:t=>"button"in t&&t.button!==2,exact:(t,e)=>Xg.some(n=>t[`${n}Key`]&&!e.includes(n))},VS=(t,e)=>{const n=t._withMods||(t._withMods={}),s=e.join(".");return n[s]||(n[s]=(i,...r)=>{for(let o=0;o<e.length;o++){const a=Zg[e[o]];if(a&&a(i,e))return}return t(i,...r)})},em=ke({patchProp:Yg},Dg);let fc;function sd(){return fc||(fc=ig(em))}const BS=(...t)=>{sd().render(...t)},tm=(...t)=>{const e=sd().createApp(...t),{mount:n}=e;return e.mount=s=>{const i=sm(s);if(!i)return;const r=e._component;!$(r)&&!r.render&&!r.template&&(r.template=i.innerHTML),i.nodeType===1&&(i.textContent="");const o=n(i,!1,nm(i));return i instanceof Element&&(i.removeAttribute("v-cloak"),i.setAttribute("data-v-app","")),o},e};function nm(t){if(t instanceof SVGElement)return"svg";if(typeof MathMLElement=="function"&&t instanceof MathMLElement)return"mathml"}function sm(t){return ye(t)?document.querySelector(t):t}/*!
 * pinia v2.3.1
 * (c) 2025 Eduardo San Martin Morote
 * @license MIT
 */let id;const Br=t=>id=t,rd=Symbol();function jo(t){return t&&typeof t=="object"&&Object.prototype.toString.call(t)==="[object Object]"&&typeof t.toJSON!="function"}var Vs;(function(t){t.direct="direct",t.patchObject="patch object",t.patchFunction="patch function"})(Vs||(Vs={}));function im(){const t=Zu(!0),e=t.run(()=>Mr({}));let n=[],s=[];const i=Aa({install(r){Br(i),i._a=r,r.provide(rd,i),r.config.globalProperties.$pinia=i,s.forEach(o=>n.push(o)),s=[]},use(r){return this._a?n.push(r):s.push(r),this},_p:n,_a:null,_e:t,_s:new Map,state:e});return i}const od=()=>{};function pc(t,e,n,s=od){t.push(e);const i=()=>{const r=t.indexOf(e);r>-1&&(t.splice(r,1),s())};return!n&&eh()&&Qp(i),i}function zn(t,...e){t.slice().forEach(n=>{n(...e)})}const rm=t=>t(),_c=Symbol(),go=Symbol();function Go(t,e){t instanceof Map&&e instanceof Map?e.forEach((n,s)=>t.set(s,n)):t instanceof Set&&e instanceof Set&&e.forEach(t.add,t);for(const n in e){if(!e.hasOwnProperty(n))continue;const s=e[n],i=t[n];jo(i)&&jo(s)&&t.hasOwnProperty(n)&&!ve(s)&&!sn(s)?t[n]=Go(i,s):t[n]=s}return t}const om=Symbol();function am(t){return!jo(t)||!t.hasOwnProperty(om)}const{assign:zt}=Object;function lm(t){return!!(ve(t)&&t.effect)}function cm(t,e,n,s){const{state:i,actions:r,getters:o}=e,a=n.state.value[t];let l;function c(){a||(n.state.value[t]=i?i():{});const h=y_(n.state.value[t]);return zt(h,r,Object.keys(o||{}).reduce((u,d)=>(u[d]=Aa(Xe(()=>{Br(n);const _=n._s.get(t);return o[d].call(_,_)})),u),{}))}return l=ad(t,c,e,n,s,!0),l}function ad(t,e,n={},s,i,r){let o;const a=zt({actions:{}},n),l={deep:!0};let c,h,u=[],d=[],_;const m=s.state.value[t];!r&&!m&&(s.state.value[t]={}),Mr({});let I;function O(q){let K;c=h=!1,typeof q=="function"?(q(s.state.value[t]),K={type:Vs.patchFunction,storeId:t,events:_}):(Go(s.state.value[t],q),K={type:Vs.patchObject,payload:q,storeId:t,events:_});const fe=I=Symbol();Fr().then(()=>{I===fe&&(c=!0)}),h=!0,zn(u,K,s.state.value[t])}const D=r?function(){const{state:K}=n,fe=K?K():{};this.$patch(Oe=>{zt(Oe,fe)})}:od;function k(){o.stop(),u=[],d=[],s._s.delete(t)}const M=(q,K="")=>{if(_c in q)return q[go]=K,q;const fe=function(){Br(s);const Oe=Array.from(arguments),Ke=[],We=[];function En(G){Ke.push(G)}function $t(G){We.push(G)}zn(d,{args:Oe,name:fe[go],store:j,after:En,onError:$t});let _e;try{_e=q.apply(this&&this.$id===t?this:j,Oe)}catch(G){throw zn(We,G),G}return _e instanceof Promise?_e.then(G=>(zn(Ke,G),G)).catch(G=>(zn(We,G),Promise.reject(G))):(zn(Ke,_e),_e)};return fe[_c]=!0,fe[go]=K,fe},x={_p:s,$id:t,$onAction:pc.bind(null,d),$patch:O,$reset:D,$subscribe(q,K={}){const fe=pc(u,q,K.detached,()=>Oe()),Oe=o.run(()=>ns(()=>s.state.value[t],Ke=>{(K.flush==="sync"?h:c)&&q({storeId:t,type:Vs.direct,events:_},Ke)},zt({},l,K)));return fe},$dispose:k},j=mi(x);s._s.set(t,j);const X=(s._a&&s._a.runWithContext||rm)(()=>s._e.run(()=>(o=Zu()).run(()=>e({action:M}))));for(const q in X){const K=X[q];if(ve(K)&&!lm(K)||sn(K))r||(m&&am(K)&&(ve(K)?K.value=m[q]:Go(K,m[q])),s.state.value[t][q]=K);else if(typeof K=="function"){const fe=M(K,q);X[q]=fe,a.actions[q]=K}}return zt(j,X),zt(Z(j),X),Object.defineProperty(j,"$state",{get:()=>s.state.value[t],set:q=>{O(K=>{zt(K,q)})}}),s._p.forEach(q=>{zt(j,o.run(()=>q({store:j,app:s._a,pinia:s,options:a})))}),m&&r&&n.hydrate&&n.hydrate(j.$state,m),c=!0,h=!0,j}/*! #__NO_SIDE_EFFECTS__ */function um(t,e,n){let s,i;const r=typeof e=="function";s=t,i=r?n:e;function o(a,l){const c=J_();return a=a||(c?et(rd,null):null),a&&Br(a),a=id,a._s.has(s)||(r?ad(s,e,i,a):cm(s,i,a)),a._s.get(s)}return o.$id=s,o}/*!
  * vue-router v4.5.1
  * (c) 2025 Eduardo San Martin Morote
  * @license MIT
  */const Jn=typeof document<"u";function ld(t){return typeof t=="object"||"displayName"in t||"props"in t||"__vccOpts"in t}function hm(t){return t.__esModule||t[Symbol.toStringTag]==="Module"||t.default&&ld(t.default)}const ne=Object.assign;function mo(t,e){const n={};for(const s in e){const i=e[s];n[s]=ut(i)?i.map(t):t(i)}return n}const Bs=()=>{},ut=Array.isArray,cd=/#/g,dm=/&/g,fm=/\//g,pm=/=/g,_m=/\?/g,ud=/\+/g,gm=/%5B/g,mm=/%5D/g,hd=/%5E/g,vm=/%60/g,dd=/%7B/g,ym=/%7C/g,fd=/%7D/g,Em=/%20/g;function Da(t){return encodeURI(""+t).replace(ym,"|").replace(gm,"[").replace(mm,"]")}function wm(t){return Da(t).replace(dd,"{").replace(fd,"}").replace(hd,"^")}function Ko(t){return Da(t).replace(ud,"%2B").replace(Em,"+").replace(cd,"%23").replace(dm,"%26").replace(vm,"`").replace(dd,"{").replace(fd,"}").replace(hd,"^")}function Im(t){return Ko(t).replace(pm,"%3D")}function bm(t){return Da(t).replace(cd,"%23").replace(_m,"%3F")}function Cm(t){return t==null?"":bm(t).replace(fm,"%2F")}function ti(t){try{return decodeURIComponent(""+t)}catch{}return""+t}const Sm=/\/$/,Tm=t=>t.replace(Sm,"");function vo(t,e,n="/"){let s,i={},r="",o="";const a=e.indexOf("#");let l=e.indexOf("?");return a<l&&a>=0&&(l=-1),l>-1&&(s=e.slice(0,l),r=e.slice(l+1,a>-1?a:e.length),i=t(r)),a>-1&&(s=s||e.slice(0,a),o=e.slice(a,e.length)),s=Nm(s??e,n),{fullPath:s+(r&&"?")+r+o,path:s,query:i,hash:ti(o)}}function Rm(t,e){const n=e.query?t(e.query):"";return e.path+(n&&"?")+n+(e.hash||"")}function gc(t,e){return!e||!t.toLowerCase().startsWith(e.toLowerCase())?t:t.slice(e.length)||"/"}function Pm(t,e,n){const s=e.matched.length-1,i=n.matched.length-1;return s>-1&&s===i&&us(e.matched[s],n.matched[i])&&pd(e.params,n.params)&&t(e.query)===t(n.query)&&e.hash===n.hash}function us(t,e){return(t.aliasOf||t)===(e.aliasOf||e)}function pd(t,e){if(Object.keys(t).length!==Object.keys(e).length)return!1;for(const n in t)if(!Am(t[n],e[n]))return!1;return!0}function Am(t,e){return ut(t)?mc(t,e):ut(e)?mc(e,t):t===e}function mc(t,e){return ut(e)?t.length===e.length&&t.every((n,s)=>n===e[s]):t.length===1&&t[0]===e}function Nm(t,e){if(t.startsWith("/"))return t;if(!t)return e;const n=e.split("/"),s=t.split("/"),i=s[s.length-1];(i===".."||i===".")&&s.push("");let r=n.length-1,o,a;for(o=0;o<s.length;o++)if(a=s[o],a!==".")if(a==="..")r>1&&r--;else break;return n.slice(0,r).join("/")+"/"+s.slice(o).join("/")}const Gt={path:"/",name:void 0,params:{},query:{},hash:"",fullPath:"/",matched:[],meta:{},redirectedFrom:void 0};var ni;(function(t){t.pop="pop",t.push="push"})(ni||(ni={}));var $s;(function(t){t.back="back",t.forward="forward",t.unknown=""})($s||($s={}));function km(t){if(!t)if(Jn){const e=document.querySelector("base");t=e&&e.getAttribute("href")||"/",t=t.replace(/^\w+:\/\/[^\/]+/,"")}else t="/";return t[0]!=="/"&&t[0]!=="#"&&(t="/"+t),Tm(t)}const Om=/^[^#]+#/;function xm(t,e){return t.replace(Om,"#")+e}function Dm(t,e){const n=document.documentElement.getBoundingClientRect(),s=t.getBoundingClientRect();return{behavior:e.behavior,left:s.left-n.left-(e.left||0),top:s.top-n.top-(e.top||0)}}const $r=()=>({left:window.scrollX,top:window.scrollY});function Mm(t){let e;if("el"in t){const n=t.el,s=typeof n=="string"&&n.startsWith("#"),i=typeof n=="string"?s?document.getElementById(n.slice(1)):document.querySelector(n):n;if(!i)return;e=Dm(i,t)}else e=t;"scrollBehavior"in document.documentElement.style?window.scrollTo(e):window.scrollTo(e.left!=null?e.left:window.scrollX,e.top!=null?e.top:window.scrollY)}function vc(t,e){return(history.state?history.state.position-e:-1)+t}const qo=new Map;function Lm(t,e){qo.set(t,e)}function Fm(t){const e=qo.get(t);return qo.delete(t),e}let Um=()=>location.protocol+"//"+location.host;function _d(t,e){const{pathname:n,search:s,hash:i}=e,r=t.indexOf("#");if(r>-1){let a=i.includes(t.slice(r))?t.slice(r).length:1,l=i.slice(a);return l[0]!=="/"&&(l="/"+l),gc(l,"")}return gc(n,t)+s+i}function Wm(t,e,n,s){let i=[],r=[],o=null;const a=({state:d})=>{const _=_d(t,location),m=n.value,I=e.value;let O=0;if(d){if(n.value=_,e.value=d,o&&o===m){o=null;return}O=I?d.position-I.position:0}else s(_);i.forEach(D=>{D(n.value,m,{delta:O,type:ni.pop,direction:O?O>0?$s.forward:$s.back:$s.unknown})})};function l(){o=n.value}function c(d){i.push(d);const _=()=>{const m=i.indexOf(d);m>-1&&i.splice(m,1)};return r.push(_),_}function h(){const{history:d}=window;d.state&&d.replaceState(ne({},d.state,{scroll:$r()}),"")}function u(){for(const d of r)d();r=[],window.removeEventListener("popstate",a),window.removeEventListener("beforeunload",h)}return window.addEventListener("popstate",a),window.addEventListener("beforeunload",h,{passive:!0}),{pauseListeners:l,listen:c,destroy:u}}function yc(t,e,n,s=!1,i=!1){return{back:t,current:e,forward:n,replaced:s,position:window.history.length,scroll:i?$r():null}}function Hm(t){const{history:e,location:n}=window,s={value:_d(t,n)},i={value:e.state};i.value||r(s.value,{back:null,current:s.value,forward:null,position:e.length-1,replaced:!0,scroll:null},!0);function r(l,c,h){const u=t.indexOf("#"),d=u>-1?(n.host&&document.querySelector("base")?t:t.slice(u))+l:Um()+t+l;try{e[h?"replaceState":"pushState"](c,"",d),i.value=c}catch(_){console.error(_),n[h?"replace":"assign"](d)}}function o(l,c){const h=ne({},e.state,yc(i.value.back,l,i.value.forward,!0),c,{position:i.value.position});r(l,h,!0),s.value=l}function a(l,c){const h=ne({},i.value,e.state,{forward:l,scroll:$r()});r(h.current,h,!0);const u=ne({},yc(s.value,l,null),{position:h.position+1},c);r(l,u,!1),s.value=l}return{location:s,state:i,push:a,replace:o}}function Vm(t){t=km(t);const e=Hm(t),n=Wm(t,e.state,e.location,e.replace);function s(r,o=!0){o||n.pauseListeners(),history.go(r)}const i=ne({location:"",base:t,go:s,createHref:xm.bind(null,t)},e,n);return Object.defineProperty(i,"location",{enumerable:!0,get:()=>e.location.value}),Object.defineProperty(i,"state",{enumerable:!0,get:()=>e.state.value}),i}function Bm(t){return typeof t=="string"||t&&typeof t=="object"}function gd(t){return typeof t=="string"||typeof t=="symbol"}const md=Symbol("");var Ec;(function(t){t[t.aborted=4]="aborted",t[t.cancelled=8]="cancelled",t[t.duplicated=16]="duplicated"})(Ec||(Ec={}));function hs(t,e){return ne(new Error,{type:t,[md]:!0},e)}function St(t,e){return t instanceof Error&&md in t&&(e==null||!!(t.type&e))}const wc="[^/]+?",$m={sensitive:!1,strict:!1,start:!0,end:!0},jm=/[.+*?^${}()[\]/\\]/g;function Gm(t,e){const n=ne({},$m,e),s=[];let i=n.start?"^":"";const r=[];for(const c of t){const h=c.length?[]:[90];n.strict&&!c.length&&(i+="/");for(let u=0;u<c.length;u++){const d=c[u];let _=40+(n.sensitive?.25:0);if(d.type===0)u||(i+="/"),i+=d.value.replace(jm,"\\$&"),_+=40;else if(d.type===1){const{value:m,repeatable:I,optional:O,regexp:D}=d;r.push({name:m,repeatable:I,optional:O});const k=D||wc;if(k!==wc){_+=10;try{new RegExp(`(${k})`)}catch(x){throw new Error(`Invalid custom RegExp for param "${m}" (${k}): `+x.message)}}let M=I?`((?:${k})(?:/(?:${k}))*)`:`(${k})`;u||(M=O&&c.length<2?`(?:/${M})`:"/"+M),O&&(M+="?"),i+=M,_+=20,O&&(_+=-8),I&&(_+=-20),k===".*"&&(_+=-50)}h.push(_)}s.push(h)}if(n.strict&&n.end){const c=s.length-1;s[c][s[c].length-1]+=.7000000000000001}n.strict||(i+="/?"),n.end?i+="$":n.strict&&!i.endsWith("/")&&(i+="(?:/|$)");const o=new RegExp(i,n.sensitive?"":"i");function a(c){const h=c.match(o),u={};if(!h)return null;for(let d=1;d<h.length;d++){const _=h[d]||"",m=r[d-1];u[m.name]=_&&m.repeatable?_.split("/"):_}return u}function l(c){let h="",u=!1;for(const d of t){(!u||!h.endsWith("/"))&&(h+="/"),u=!1;for(const _ of d)if(_.type===0)h+=_.value;else if(_.type===1){const{value:m,repeatable:I,optional:O}=_,D=m in c?c[m]:"";if(ut(D)&&!I)throw new Error(`Provided param "${m}" is an array but it is not repeatable (* or + modifiers)`);const k=ut(D)?D.join("/"):D;if(!k)if(O)d.length<2&&(h.endsWith("/")?h=h.slice(0,-1):u=!0);else throw new Error(`Missing required param "${m}"`);h+=k}}return h||"/"}return{re:o,score:s,keys:r,parse:a,stringify:l}}function Km(t,e){let n=0;for(;n<t.length&&n<e.length;){const s=e[n]-t[n];if(s)return s;n++}return t.length<e.length?t.length===1&&t[0]===80?-1:1:t.length>e.length?e.length===1&&e[0]===80?1:-1:0}function vd(t,e){let n=0;const s=t.score,i=e.score;for(;n<s.length&&n<i.length;){const r=Km(s[n],i[n]);if(r)return r;n++}if(Math.abs(i.length-s.length)===1){if(Ic(s))return 1;if(Ic(i))return-1}return i.length-s.length}function Ic(t){const e=t[t.length-1];return t.length>0&&e[e.length-1]<0}const qm={type:0,value:""},zm=/[a-zA-Z0-9_]/;function Ym(t){if(!t)return[[]];if(t==="/")return[[qm]];if(!t.startsWith("/"))throw new Error(`Invalid path "${t}"`);function e(_){throw new Error(`ERR (${n})/"${c}": ${_}`)}let n=0,s=n;const i=[];let r;function o(){r&&i.push(r),r=[]}let a=0,l,c="",h="";function u(){c&&(n===0?r.push({type:0,value:c}):n===1||n===2||n===3?(r.length>1&&(l==="*"||l==="+")&&e(`A repeatable param (${c}) must be alone in its segment. eg: '/:ids+.`),r.push({type:1,value:c,regexp:h,repeatable:l==="*"||l==="+",optional:l==="*"||l==="?"})):e("Invalid state to consume buffer"),c="")}function d(){c+=l}for(;a<t.length;){if(l=t[a++],l==="\\"&&n!==2){s=n,n=4;continue}switch(n){case 0:l==="/"?(c&&u(),o()):l===":"?(u(),n=1):d();break;case 4:d(),n=s;break;case 1:l==="("?n=2:zm.test(l)?d():(u(),n=0,l!=="*"&&l!=="?"&&l!=="+"&&a--);break;case 2:l===")"?h[h.length-1]=="\\"?h=h.slice(0,-1)+l:n=3:h+=l;break;case 3:u(),n=0,l!=="*"&&l!=="?"&&l!=="+"&&a--,h="";break;default:e("Unknown state");break}}return n===2&&e(`Unfinished custom RegExp for param "${c}"`),u(),o(),i}function Qm(t,e,n){const s=Gm(Ym(t.path),n),i=ne(s,{record:t,parent:e,children:[],alias:[]});return e&&!i.record.aliasOf==!e.record.aliasOf&&e.children.push(i),i}function Jm(t,e){const n=[],s=new Map;e=Tc({strict:!1,end:!0,sensitive:!1},e);function i(u){return s.get(u)}function r(u,d,_){const m=!_,I=Cc(u);I.aliasOf=_&&_.record;const O=Tc(e,u),D=[I];if("alias"in u){const x=typeof u.alias=="string"?[u.alias]:u.alias;for(const j of x)D.push(Cc(ne({},I,{components:_?_.record.components:I.components,path:j,aliasOf:_?_.record:I})))}let k,M;for(const x of D){const{path:j}=x;if(d&&j[0]!=="/"){const ge=d.record.path,X=ge[ge.length-1]==="/"?"":"/";x.path=d.record.path+(j&&X+j)}if(k=Qm(x,d,O),_?_.alias.push(k):(M=M||k,M!==k&&M.alias.push(k),m&&u.name&&!Sc(k)&&o(u.name)),yd(k)&&l(k),I.children){const ge=I.children;for(let X=0;X<ge.length;X++)r(ge[X],k,_&&_.children[X])}_=_||k}return M?()=>{o(M)}:Bs}function o(u){if(gd(u)){const d=s.get(u);d&&(s.delete(u),n.splice(n.indexOf(d),1),d.children.forEach(o),d.alias.forEach(o))}else{const d=n.indexOf(u);d>-1&&(n.splice(d,1),u.record.name&&s.delete(u.record.name),u.children.forEach(o),u.alias.forEach(o))}}function a(){return n}function l(u){const d=ev(u,n);n.splice(d,0,u),u.record.name&&!Sc(u)&&s.set(u.record.name,u)}function c(u,d){let _,m={},I,O;if("name"in u&&u.name){if(_=s.get(u.name),!_)throw hs(1,{location:u});O=_.record.name,m=ne(bc(d.params,_.keys.filter(M=>!M.optional).concat(_.parent?_.parent.keys.filter(M=>M.optional):[]).map(M=>M.name)),u.params&&bc(u.params,_.keys.map(M=>M.name))),I=_.stringify(m)}else if(u.path!=null)I=u.path,_=n.find(M=>M.re.test(I)),_&&(m=_.parse(I),O=_.record.name);else{if(_=d.name?s.get(d.name):n.find(M=>M.re.test(d.path)),!_)throw hs(1,{location:u,currentLocation:d});O=_.record.name,m=ne({},d.params,u.params),I=_.stringify(m)}const D=[];let k=_;for(;k;)D.unshift(k.record),k=k.parent;return{name:O,path:I,params:m,matched:D,meta:Zm(D)}}t.forEach(u=>r(u));function h(){n.length=0,s.clear()}return{addRoute:r,resolve:c,removeRoute:o,clearRoutes:h,getRoutes:a,getRecordMatcher:i}}function bc(t,e){const n={};for(const s of e)s in t&&(n[s]=t[s]);return n}function Cc(t){const e={path:t.path,redirect:t.redirect,name:t.name,meta:t.meta||{},aliasOf:t.aliasOf,beforeEnter:t.beforeEnter,props:Xm(t),children:t.children||[],instances:{},leaveGuards:new Set,updateGuards:new Set,enterCallbacks:{},components:"components"in t?t.components||null:t.component&&{default:t.component}};return Object.defineProperty(e,"mods",{value:{}}),e}function Xm(t){const e={},n=t.props||!1;if("component"in t)e.default=n;else for(const s in t.components)e[s]=typeof n=="object"?n[s]:n;return e}function Sc(t){for(;t;){if(t.record.aliasOf)return!0;t=t.parent}return!1}function Zm(t){return t.reduce((e,n)=>ne(e,n.meta),{})}function Tc(t,e){const n={};for(const s in t)n[s]=s in e?e[s]:t[s];return n}function ev(t,e){let n=0,s=e.length;for(;n!==s;){const r=n+s>>1;vd(t,e[r])<0?s=r:n=r+1}const i=tv(t);return i&&(s=e.lastIndexOf(i,s-1)),s}function tv(t){let e=t;for(;e=e.parent;)if(yd(e)&&vd(t,e)===0)return e}function yd({record:t}){return!!(t.name||t.components&&Object.keys(t.components).length||t.redirect)}function nv(t){const e={};if(t===""||t==="?")return e;const s=(t[0]==="?"?t.slice(1):t).split("&");for(let i=0;i<s.length;++i){const r=s[i].replace(ud," "),o=r.indexOf("="),a=ti(o<0?r:r.slice(0,o)),l=o<0?null:ti(r.slice(o+1));if(a in e){let c=e[a];ut(c)||(c=e[a]=[c]),c.push(l)}else e[a]=l}return e}function Rc(t){let e="";for(let n in t){const s=t[n];if(n=Im(n),s==null){s!==void 0&&(e+=(e.length?"&":"")+n);continue}(ut(s)?s.map(r=>r&&Ko(r)):[s&&Ko(s)]).forEach(r=>{r!==void 0&&(e+=(e.length?"&":"")+n,r!=null&&(e+="="+r))})}return e}function sv(t){const e={};for(const n in t){const s=t[n];s!==void 0&&(e[n]=ut(s)?s.map(i=>i==null?null:""+i):s==null?s:""+s)}return e}const iv=Symbol(""),Pc=Symbol(""),jr=Symbol(""),Ma=Symbol(""),zo=Symbol("");function Rs(){let t=[];function e(s){return t.push(s),()=>{const i=t.indexOf(s);i>-1&&t.splice(i,1)}}function n(){t=[]}return{add:e,list:()=>t.slice(),reset:n}}function Jt(t,e,n,s,i,r=o=>o()){const o=s&&(s.enterCallbacks[i]=s.enterCallbacks[i]||[]);return()=>new Promise((a,l)=>{const c=d=>{d===!1?l(hs(4,{from:n,to:e})):d instanceof Error?l(d):Bm(d)?l(hs(2,{from:e,to:d})):(o&&s.enterCallbacks[i]===o&&typeof d=="function"&&o.push(d),a())},h=r(()=>t.call(s&&s.instances[i],e,n,c));let u=Promise.resolve(h);t.length<3&&(u=u.then(c)),u.catch(d=>l(d))})}function yo(t,e,n,s,i=r=>r()){const r=[];for(const o of t)for(const a in o.components){let l=o.components[a];if(!(e!=="beforeRouteEnter"&&!o.instances[a]))if(ld(l)){const h=(l.__vccOpts||l)[e];h&&r.push(Jt(h,n,s,o,a,i))}else{let c=l();r.push(()=>c.then(h=>{if(!h)throw new Error(`Couldn't resolve component "${a}" at "${o.path}"`);const u=hm(h)?h.default:h;o.mods[a]=h,o.components[a]=u;const _=(u.__vccOpts||u)[e];return _&&Jt(_,n,s,o,a,i)()}))}}return r}function Ac(t){const e=et(jr),n=et(Ma),s=Xe(()=>{const l=rn(t.to);return e.resolve(l)}),i=Xe(()=>{const{matched:l}=s.value,{length:c}=l,h=l[c-1],u=n.matched;if(!h||!u.length)return-1;const d=u.findIndex(us.bind(null,h));if(d>-1)return d;const _=Nc(l[c-2]);return c>1&&Nc(h)===_&&u[u.length-1].path!==_?u.findIndex(us.bind(null,l[c-2])):d}),r=Xe(()=>i.value>-1&&cv(n.params,s.value.params)),o=Xe(()=>i.value>-1&&i.value===n.matched.length-1&&pd(n.params,s.value.params));function a(l={}){if(lv(l)){const c=e[rn(t.replace)?"replace":"push"](rn(t.to)).catch(Bs);return t.viewTransition&&typeof document<"u"&&"startViewTransition"in document&&document.startViewTransition(()=>c),c}return Promise.resolve()}return{route:s,href:Xe(()=>s.value.href),isActive:r,isExactActive:o,navigate:a}}function rv(t){return t.length===1?t[0]:t}const ov=Ch({name:"RouterLink",compatConfig:{MODE:3},props:{to:{type:[String,Object],required:!0},replace:Boolean,activeClass:String,exactActiveClass:String,custom:Boolean,ariaCurrentValue:{type:String,default:"page"},viewTransition:Boolean},useLink:Ac,setup(t,{slots:e}){const n=mi(Ac(t)),{options:s}=et(jr),i=Xe(()=>({[kc(t.activeClass,s.linkActiveClass,"router-link-active")]:n.isActive,[kc(t.exactActiveClass,s.linkExactActiveClass,"router-link-exact-active")]:n.isExactActive}));return()=>{const r=e.default&&rv(e.default(n));return t.custom?r:td("a",{"aria-current":n.isExactActive?t.ariaCurrentValue:null,href:n.href,onClick:n.navigate,class:i.value},r)}}}),av=ov;function lv(t){if(!(t.metaKey||t.altKey||t.ctrlKey||t.shiftKey)&&!t.defaultPrevented&&!(t.button!==void 0&&t.button!==0)){if(t.currentTarget&&t.currentTarget.getAttribute){const e=t.currentTarget.getAttribute("target");if(/\b_blank\b/i.test(e))return}return t.preventDefault&&t.preventDefault(),!0}}function cv(t,e){for(const n in e){const s=e[n],i=t[n];if(typeof s=="string"){if(s!==i)return!1}else if(!ut(i)||i.length!==s.length||s.some((r,o)=>r!==i[o]))return!1}return!0}function Nc(t){return t?t.aliasOf?t.aliasOf.path:t.path:""}const kc=(t,e,n)=>t??e??n,uv=Ch({name:"RouterView",inheritAttrs:!1,props:{name:{type:String,default:"default"},route:Object},compatConfig:{MODE:3},setup(t,{attrs:e,slots:n}){const s=et(zo),i=Xe(()=>t.route||s.value),r=et(Pc,0),o=Xe(()=>{let c=rn(r);const{matched:h}=i.value;let u;for(;(u=h[c])&&!u.components;)c++;return c}),a=Xe(()=>i.value.matched[o.value]);Wi(Pc,Xe(()=>o.value+1)),Wi(iv,a),Wi(zo,i);const l=Mr();return ns(()=>[l.value,a.value,t.name],([c,h,u],[d,_,m])=>{h&&(h.instances[u]=c,_&&_!==h&&c&&c===d&&(h.leaveGuards.size||(h.leaveGuards=_.leaveGuards),h.updateGuards.size||(h.updateGuards=_.updateGuards))),c&&h&&(!_||!us(h,_)||!d)&&(h.enterCallbacks[u]||[]).forEach(I=>I(c))},{flush:"post"}),()=>{const c=i.value,h=t.name,u=a.value,d=u&&u.components[h];if(!d)return Oc(n.default,{Component:d,route:c});const _=u.props[h],m=_?_===!0?c.params:typeof _=="function"?_(c):_:null,O=td(d,ne({},m,e,{onVnodeUnmounted:D=>{D.component.isUnmounted&&(u.instances[h]=null)},ref:l}));return Oc(n.default,{Component:O,route:c})||O}}});function Oc(t,e){if(!t)return null;const n=t(e);return n.length===1?n[0]:n}const hv=uv;function dv(t){const e=Jm(t.routes,t),n=t.parseQuery||nv,s=t.stringifyQuery||Rc,i=t.history,r=Rs(),o=Rs(),a=Rs(),l=g_(Gt);let c=Gt;Jn&&t.scrollBehavior&&"scrollRestoration"in history&&(history.scrollRestoration="manual");const h=mo.bind(null,y=>""+y),u=mo.bind(null,Cm),d=mo.bind(null,ti);function _(y,N){let P,L;return gd(y)?(P=e.getRecordMatcher(y),L=N):L=y,e.addRoute(L,P)}function m(y){const N=e.getRecordMatcher(y);N&&e.removeRoute(N)}function I(){return e.getRoutes().map(y=>y.record)}function O(y){return!!e.getRecordMatcher(y)}function D(y,N){if(N=ne({},N||l.value),typeof y=="string"){const g=vo(n,y,N.path),v=e.resolve({path:g.path},N),w=i.createHref(g.fullPath);return ne(g,v,{params:d(v.params),hash:ti(g.hash),redirectedFrom:void 0,href:w})}let P;if(y.path!=null)P=ne({},y,{path:vo(n,y.path,N.path).path});else{const g=ne({},y.params);for(const v in g)g[v]==null&&delete g[v];P=ne({},y,{params:u(g)}),N.params=u(N.params)}const L=e.resolve(P,N),oe=y.hash||"";L.params=h(d(L.params));const f=Rm(s,ne({},y,{hash:wm(oe),path:L.path})),p=i.createHref(f);return ne({fullPath:f,hash:oe,query:s===Rc?sv(y.query):y.query||{}},L,{redirectedFrom:void 0,href:p})}function k(y){return typeof y=="string"?vo(n,y,l.value.path):ne({},y)}function M(y,N){if(c!==y)return hs(8,{from:N,to:y})}function x(y){return X(y)}function j(y){return x(ne(k(y),{replace:!0}))}function ge(y){const N=y.matched[y.matched.length-1];if(N&&N.redirect){const{redirect:P}=N;let L=typeof P=="function"?P(y):P;return typeof L=="string"&&(L=L.includes("?")||L.includes("#")?L=k(L):{path:L},L.params={}),ne({query:y.query,hash:y.hash,params:L.path!=null?{}:y.params},L)}}function X(y,N){const P=c=D(y),L=l.value,oe=y.state,f=y.force,p=y.replace===!0,g=ge(P);if(g)return X(ne(k(g),{state:typeof g=="object"?ne({},oe,g.state):oe,force:f,replace:p}),N||P);const v=P;v.redirectedFrom=N;let w;return!f&&Pm(s,L,P)&&(w=hs(16,{to:v,from:L}),ht(L,L,!0,!1)),(w?Promise.resolve(w):fe(v,L)).catch(E=>St(E)?St(E,2)?E:jt(E):te(E,v,L)).then(E=>{if(E){if(St(E,2))return X(ne({replace:p},k(E.to),{state:typeof E.to=="object"?ne({},oe,E.to.state):oe,force:f}),N||v)}else E=Ke(v,L,!0,p,oe);return Oe(v,L,E),E})}function q(y,N){const P=M(y,N);return P?Promise.reject(P):Promise.resolve()}function K(y){const N=Gn.values().next().value;return N&&typeof N.runWithContext=="function"?N.runWithContext(y):y()}function fe(y,N){let P;const[L,oe,f]=fv(y,N);P=yo(L.reverse(),"beforeRouteLeave",y,N);for(const g of L)g.leaveGuards.forEach(v=>{P.push(Jt(v,y,N))});const p=q.bind(null,y,N);return P.push(p),Je(P).then(()=>{P=[];for(const g of r.list())P.push(Jt(g,y,N));return P.push(p),Je(P)}).then(()=>{P=yo(oe,"beforeRouteUpdate",y,N);for(const g of oe)g.updateGuards.forEach(v=>{P.push(Jt(v,y,N))});return P.push(p),Je(P)}).then(()=>{P=[];for(const g of f)if(g.beforeEnter)if(ut(g.beforeEnter))for(const v of g.beforeEnter)P.push(Jt(v,y,N));else P.push(Jt(g.beforeEnter,y,N));return P.push(p),Je(P)}).then(()=>(y.matched.forEach(g=>g.enterCallbacks={}),P=yo(f,"beforeRouteEnter",y,N,K),P.push(p),Je(P))).then(()=>{P=[];for(const g of o.list())P.push(Jt(g,y,N));return P.push(p),Je(P)}).catch(g=>St(g,8)?g:Promise.reject(g))}function Oe(y,N,P){a.list().forEach(L=>K(()=>L(y,N,P)))}function Ke(y,N,P,L,oe){const f=M(y,N);if(f)return f;const p=N===Gt,g=Jn?history.state:{};P&&(L||p?i.replace(y.fullPath,ne({scroll:p&&g&&g.scroll},oe)):i.push(y.fullPath,oe)),l.value=y,ht(y,N,P,p),jt()}let We;function En(){We||(We=i.listen((y,N,P)=>{if(!ki.listening)return;const L=D(y),oe=ge(L);if(oe){X(ne(oe,{replace:!0,force:!0}),L).catch(Bs);return}c=L;const f=l.value;Jn&&Lm(vc(f.fullPath,P.delta),$r()),fe(L,f).catch(p=>St(p,12)?p:St(p,2)?(X(ne(k(p.to),{force:!0}),L).then(g=>{St(g,20)&&!P.delta&&P.type===ni.pop&&i.go(-1,!1)}).catch(Bs),Promise.reject()):(P.delta&&i.go(-P.delta,!1),te(p,L,f))).then(p=>{p=p||Ke(L,f,!1),p&&(P.delta&&!St(p,8)?i.go(-P.delta,!1):P.type===ni.pop&&St(p,20)&&i.go(-1,!1)),Oe(L,f,p)}).catch(Bs)}))}let $t=Rs(),_e=Rs(),G;function te(y,N,P){jt(y);const L=_e.list();return L.length?L.forEach(oe=>oe(y,N,P)):console.error(y),Promise.reject(y)}function bt(){return G&&l.value!==Gt?Promise.resolve():new Promise((y,N)=>{$t.add([y,N])})}function jt(y){return G||(G=!y,En(),$t.list().forEach(([N,P])=>y?P(y):N()),$t.reset()),y}function ht(y,N,P,L){const{scrollBehavior:oe}=t;if(!Jn||!oe)return Promise.resolve();const f=!P&&Fm(vc(y.fullPath,0))||(L||!P)&&history.state&&history.state.scroll||null;return Fr().then(()=>oe(y,N,f)).then(p=>p&&Mm(p)).catch(p=>te(p,y,N))}const He=y=>i.go(y);let jn;const Gn=new Set,ki={currentRoute:l,listening:!0,addRoute:_,removeRoute:m,clearRoutes:e.clearRoutes,hasRoute:O,getRoutes:I,resolve:D,options:t,push:x,replace:j,go:He,back:()=>He(-1),forward:()=>He(1),beforeEach:r.add,beforeResolve:o.add,afterEach:a.add,onError:_e.add,isReady:bt,install(y){const N=this;y.component("RouterLink",av),y.component("RouterView",hv),y.config.globalProperties.$router=N,Object.defineProperty(y.config.globalProperties,"$route",{enumerable:!0,get:()=>rn(l)}),Jn&&!jn&&l.value===Gt&&(jn=!0,x(i.location).catch(oe=>{}));const P={};for(const oe in Gt)Object.defineProperty(P,oe,{get:()=>l.value[oe],enumerable:!0});y.provide(jr,N),y.provide(Ma,_h(P)),y.provide(zo,l);const L=y.unmount;Gn.add(y),y.unmount=function(){Gn.delete(y),Gn.size<1&&(c=Gt,We&&We(),We=null,l.value=Gt,jn=!1,G=!1),L()}}};function Je(y){return y.reduce((N,P)=>N.then(()=>K(P)),Promise.resolve())}return ki}function fv(t,e){const n=[],s=[],i=[],r=Math.max(e.matched.length,t.matched.length);for(let o=0;o<r;o++){const a=e.matched[o];a&&(t.matched.find(c=>us(c,a))?s.push(a):n.push(a));const l=t.matched[o];l&&(e.matched.find(c=>us(c,l))||i.push(l))}return[n,s,i]}function pv(){return et(jr)}function $S(t){return et(Ma)}var xc={};/**
 * @license
 * Copyright 2017 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */const Ed={NODE_ADMIN:!1,SDK_VERSION:"${JSCORE_VERSION}"};/**
 * @license
 * Copyright 2017 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */const C=function(t,e){if(!t)throw ms(e)},ms=function(t){return new Error("Firebase Database ("+Ed.SDK_VERSION+") INTERNAL ASSERT FAILED: "+t)};/**
 * @license
 * Copyright 2017 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */const wd=function(t){const e=[];let n=0;for(let s=0;s<t.length;s++){let i=t.charCodeAt(s);i<128?e[n++]=i:i<2048?(e[n++]=i>>6|192,e[n++]=i&63|128):(i&64512)===55296&&s+1<t.length&&(t.charCodeAt(s+1)&64512)===56320?(i=65536+((i&1023)<<10)+(t.charCodeAt(++s)&1023),e[n++]=i>>18|240,e[n++]=i>>12&63|128,e[n++]=i>>6&63|128,e[n++]=i&63|128):(e[n++]=i>>12|224,e[n++]=i>>6&63|128,e[n++]=i&63|128)}return e},_v=function(t){const e=[];let n=0,s=0;for(;n<t.length;){const i=t[n++];if(i<128)e[s++]=String.fromCharCode(i);else if(i>191&&i<224){const r=t[n++];e[s++]=String.fromCharCode((i&31)<<6|r&63)}else if(i>239&&i<365){const r=t[n++],o=t[n++],a=t[n++],l=((i&7)<<18|(r&63)<<12|(o&63)<<6|a&63)-65536;e[s++]=String.fromCharCode(55296+(l>>10)),e[s++]=String.fromCharCode(56320+(l&1023))}else{const r=t[n++],o=t[n++];e[s++]=String.fromCharCode((i&15)<<12|(r&63)<<6|o&63)}}return e.join("")},La={byteToCharMap_:null,charToByteMap_:null,byteToCharMapWebSafe_:null,charToByteMapWebSafe_:null,ENCODED_VALS_BASE:"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789",get ENCODED_VALS(){return this.ENCODED_VALS_BASE+"+/="},get ENCODED_VALS_WEBSAFE(){return this.ENCODED_VALS_BASE+"-_."},HAS_NATIVE_SUPPORT:typeof atob=="function",encodeByteArray(t,e){if(!Array.isArray(t))throw Error("encodeByteArray takes an array as a parameter");this.init_();const n=e?this.byteToCharMapWebSafe_:this.byteToCharMap_,s=[];for(let i=0;i<t.length;i+=3){const r=t[i],o=i+1<t.length,a=o?t[i+1]:0,l=i+2<t.length,c=l?t[i+2]:0,h=r>>2,u=(r&3)<<4|a>>4;let d=(a&15)<<2|c>>6,_=c&63;l||(_=64,o||(d=64)),s.push(n[h],n[u],n[d],n[_])}return s.join("")},encodeString(t,e){return this.HAS_NATIVE_SUPPORT&&!e?btoa(t):this.encodeByteArray(wd(t),e)},decodeString(t,e){return this.HAS_NATIVE_SUPPORT&&!e?atob(t):_v(this.decodeStringToByteArray(t,e))},decodeStringToByteArray(t,e){this.init_();const n=e?this.charToByteMapWebSafe_:this.charToByteMap_,s=[];for(let i=0;i<t.length;){const r=n[t.charAt(i++)],a=i<t.length?n[t.charAt(i)]:0;++i;const c=i<t.length?n[t.charAt(i)]:64;++i;const u=i<t.length?n[t.charAt(i)]:64;if(++i,r==null||a==null||c==null||u==null)throw new gv;const d=r<<2|a>>4;if(s.push(d),c!==64){const _=a<<4&240|c>>2;if(s.push(_),u!==64){const m=c<<6&192|u;s.push(m)}}}return s},init_(){if(!this.byteToCharMap_){this.byteToCharMap_={},this.charToByteMap_={},this.byteToCharMapWebSafe_={},this.charToByteMapWebSafe_={};for(let t=0;t<this.ENCODED_VALS.length;t++)this.byteToCharMap_[t]=this.ENCODED_VALS.charAt(t),this.charToByteMap_[this.byteToCharMap_[t]]=t,this.byteToCharMapWebSafe_[t]=this.ENCODED_VALS_WEBSAFE.charAt(t),this.charToByteMapWebSafe_[this.byteToCharMapWebSafe_[t]]=t,t>=this.ENCODED_VALS_BASE.length&&(this.charToByteMap_[this.ENCODED_VALS_WEBSAFE.charAt(t)]=t,this.charToByteMapWebSafe_[this.ENCODED_VALS.charAt(t)]=t)}}};class gv extends Error{constructor(){super(...arguments),this.name="DecodeBase64StringError"}}const Id=function(t){const e=wd(t);return La.encodeByteArray(e,!0)},rr=function(t){return Id(t).replace(/\./g,"")},or=function(t){try{return La.decodeString(t,!0)}catch(e){console.error("base64Decode failed: ",e)}return null};/**
 * @license
 * Copyright 2017 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */function mv(t){return bd(void 0,t)}function bd(t,e){if(!(e instanceof Object))return e;switch(e.constructor){case Date:const n=e;return new Date(n.getTime());case Object:t===void 0&&(t={});break;case Array:t=[];break;default:return e}for(const n in e)!e.hasOwnProperty(n)||!vv(n)||(t[n]=bd(t[n],e[n]));return t}function vv(t){return t!=="__proto__"}/**
 * @license
 * Copyright 2022 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */function yv(){if(typeof self<"u")return self;if(typeof window<"u")return window;if(typeof global<"u")return global;throw new Error("Unable to locate global object.")}/**
 * @license
 * Copyright 2022 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */const Ev=()=>yv().__FIREBASE_DEFAULTS__,wv=()=>{if(typeof process>"u"||typeof xc>"u")return;const t=xc.__FIREBASE_DEFAULTS__;if(t)return JSON.parse(t)},Iv=()=>{if(typeof document>"u")return;let t;try{t=document.cookie.match(/__FIREBASE_DEFAULTS__=([^;]+)/)}catch{return}const e=t&&or(t[1]);return e&&JSON.parse(e)},Fa=()=>{try{return Ev()||wv()||Iv()}catch(t){console.info(`Unable to get __FIREBASE_DEFAULTS__ due to: ${t}`);return}},Cd=t=>{var e,n;return(n=(e=Fa())===null||e===void 0?void 0:e.emulatorHosts)===null||n===void 0?void 0:n[t]},bv=t=>{const e=Cd(t);if(!e)return;const n=e.lastIndexOf(":");if(n<=0||n+1===e.length)throw new Error(`Invalid host ${e} with no separate hostname and port!`);const s=parseInt(e.substring(n+1),10);return e[0]==="["?[e.substring(1,n-1),s]:[e.substring(0,n),s]},Sd=()=>{var t;return(t=Fa())===null||t===void 0?void 0:t.config},Td=t=>{var e;return(e=Fa())===null||e===void 0?void 0:e[`_${t}`]};/**
 * @license
 * Copyright 2017 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */class Gr{constructor(){this.reject=()=>{},this.resolve=()=>{},this.promise=new Promise((e,n)=>{this.resolve=e,this.reject=n})}wrapCallback(e){return(n,s)=>{n?this.reject(n):this.resolve(s),typeof e=="function"&&(this.promise.catch(()=>{}),e.length===1?e(n):e(n,s))}}}/**
 * @license
 * Copyright 2021 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */function Cv(t,e){if(t.uid)throw new Error('The "uid" field is no longer supported by mockUserToken. Please use "sub" instead for Firebase Auth User ID.');const n={alg:"none",type:"JWT"},s=e||"demo-project",i=t.iat||0,r=t.sub||t.user_id;if(!r)throw new Error("mockUserToken must contain 'sub' or 'user_id' field!");const o=Object.assign({iss:`https://securetoken.google.com/${s}`,aud:s,iat:i,exp:i+3600,auth_time:i,sub:r,user_id:r,firebase:{sign_in_provider:"custom",identities:{}}},t);return[rr(JSON.stringify(n)),rr(JSON.stringify(o)),""].join(".")}/**
 * @license
 * Copyright 2017 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */function Fe(){return typeof navigator<"u"&&typeof navigator.userAgent=="string"?navigator.userAgent:""}function Ua(){return typeof window<"u"&&!!(window.cordova||window.phonegap||window.PhoneGap)&&/ios|iphone|ipod|ipad|android|blackberry|iemobile/i.test(Fe())}function Sv(){return typeof navigator<"u"&&navigator.userAgent==="Cloudflare-Workers"}function Tv(){const t=typeof chrome=="object"?chrome.runtime:typeof browser=="object"?browser.runtime:void 0;return typeof t=="object"&&t.id!==void 0}function Rd(){return typeof navigator=="object"&&navigator.product==="ReactNative"}function Rv(){const t=Fe();return t.indexOf("MSIE ")>=0||t.indexOf("Trident/")>=0}function Pv(){return Ed.NODE_ADMIN===!0}function Av(){try{return typeof indexedDB=="object"}catch{return!1}}function Nv(){return new Promise((t,e)=>{try{let n=!0;const s="validate-browser-context-for-indexeddb-analytics-module",i=self.indexedDB.open(s);i.onsuccess=()=>{i.result.close(),n||self.indexedDB.deleteDatabase(s),t(!0)},i.onupgradeneeded=()=>{n=!1},i.onerror=()=>{var r;e(((r=i.error)===null||r===void 0?void 0:r.message)||"")}}catch(n){e(n)}})}/**
 * @license
 * Copyright 2017 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */const kv="FirebaseError";class gn extends Error{constructor(e,n,s){super(n),this.code=e,this.customData=s,this.name=kv,Object.setPrototypeOf(this,gn.prototype),Error.captureStackTrace&&Error.captureStackTrace(this,Ei.prototype.create)}}class Ei{constructor(e,n,s){this.service=e,this.serviceName=n,this.errors=s}create(e,...n){const s=n[0]||{},i=`${this.service}/${e}`,r=this.errors[e],o=r?Ov(r,s):"Error",a=`${this.serviceName}: ${o} (${i}).`;return new gn(i,a,s)}}function Ov(t,e){return t.replace(xv,(n,s)=>{const i=e[s];return i!=null?String(i):`<${s}?>`})}const xv=/\{\$([^}]+)}/g;/**
 * @license
 * Copyright 2017 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */function si(t){return JSON.parse(t)}function we(t){return JSON.stringify(t)}/**
 * @license
 * Copyright 2017 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */const Pd=function(t){let e={},n={},s={},i="";try{const r=t.split(".");e=si(or(r[0])||""),n=si(or(r[1])||""),i=r[2],s=n.d||{},delete n.d}catch{}return{header:e,claims:n,data:s,signature:i}},Dv=function(t){const e=Pd(t),n=e.claims;return!!n&&typeof n=="object"&&n.hasOwnProperty("iat")},Mv=function(t){const e=Pd(t).claims;return typeof e=="object"&&e.admin===!0};/**
 * @license
 * Copyright 2017 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */function Vt(t,e){return Object.prototype.hasOwnProperty.call(t,e)}function ds(t,e){if(Object.prototype.hasOwnProperty.call(t,e))return t[e]}function Yo(t){for(const e in t)if(Object.prototype.hasOwnProperty.call(t,e))return!1;return!0}function ar(t,e,n){const s={};for(const i in t)Object.prototype.hasOwnProperty.call(t,i)&&(s[i]=e.call(n,t[i],i,t));return s}function lr(t,e){if(t===e)return!0;const n=Object.keys(t),s=Object.keys(e);for(const i of n){if(!s.includes(i))return!1;const r=t[i],o=e[i];if(Dc(r)&&Dc(o)){if(!lr(r,o))return!1}else if(r!==o)return!1}for(const i of s)if(!n.includes(i))return!1;return!0}function Dc(t){return t!==null&&typeof t=="object"}/**
 * @license
 * Copyright 2017 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */function vs(t){const e=[];for(const[n,s]of Object.entries(t))Array.isArray(s)?s.forEach(i=>{e.push(encodeURIComponent(n)+"="+encodeURIComponent(i))}):e.push(encodeURIComponent(n)+"="+encodeURIComponent(s));return e.length?"&"+e.join("&"):""}function xs(t){const e={};return t.replace(/^\?/,"").split("&").forEach(s=>{if(s){const[i,r]=s.split("=");e[decodeURIComponent(i)]=decodeURIComponent(r)}}),e}function Ds(t){const e=t.indexOf("?");if(!e)return"";const n=t.indexOf("#",e);return t.substring(e,n>0?n:void 0)}/**
 * @license
 * Copyright 2017 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */class Lv{constructor(){this.chain_=[],this.buf_=[],this.W_=[],this.pad_=[],this.inbuf_=0,this.total_=0,this.blockSize=512/8,this.pad_[0]=128;for(let e=1;e<this.blockSize;++e)this.pad_[e]=0;this.reset()}reset(){this.chain_[0]=1732584193,this.chain_[1]=4023233417,this.chain_[2]=2562383102,this.chain_[3]=271733878,this.chain_[4]=3285377520,this.inbuf_=0,this.total_=0}compress_(e,n){n||(n=0);const s=this.W_;if(typeof e=="string")for(let u=0;u<16;u++)s[u]=e.charCodeAt(n)<<24|e.charCodeAt(n+1)<<16|e.charCodeAt(n+2)<<8|e.charCodeAt(n+3),n+=4;else for(let u=0;u<16;u++)s[u]=e[n]<<24|e[n+1]<<16|e[n+2]<<8|e[n+3],n+=4;for(let u=16;u<80;u++){const d=s[u-3]^s[u-8]^s[u-14]^s[u-16];s[u]=(d<<1|d>>>31)&4294967295}let i=this.chain_[0],r=this.chain_[1],o=this.chain_[2],a=this.chain_[3],l=this.chain_[4],c,h;for(let u=0;u<80;u++){u<40?u<20?(c=a^r&(o^a),h=1518500249):(c=r^o^a,h=1859775393):u<60?(c=r&o|a&(r|o),h=2400959708):(c=r^o^a,h=3395469782);const d=(i<<5|i>>>27)+c+l+h+s[u]&4294967295;l=a,a=o,o=(r<<30|r>>>2)&4294967295,r=i,i=d}this.chain_[0]=this.chain_[0]+i&4294967295,this.chain_[1]=this.chain_[1]+r&4294967295,this.chain_[2]=this.chain_[2]+o&4294967295,this.chain_[3]=this.chain_[3]+a&4294967295,this.chain_[4]=this.chain_[4]+l&4294967295}update(e,n){if(e==null)return;n===void 0&&(n=e.length);const s=n-this.blockSize;let i=0;const r=this.buf_;let o=this.inbuf_;for(;i<n;){if(o===0)for(;i<=s;)this.compress_(e,i),i+=this.blockSize;if(typeof e=="string"){for(;i<n;)if(r[o]=e.charCodeAt(i),++o,++i,o===this.blockSize){this.compress_(r),o=0;break}}else for(;i<n;)if(r[o]=e[i],++o,++i,o===this.blockSize){this.compress_(r),o=0;break}}this.inbuf_=o,this.total_+=n}digest(){const e=[];let n=this.total_*8;this.inbuf_<56?this.update(this.pad_,56-this.inbuf_):this.update(this.pad_,this.blockSize-(this.inbuf_-56));for(let i=this.blockSize-1;i>=56;i--)this.buf_[i]=n&255,n/=256;this.compress_(this.buf_);let s=0;for(let i=0;i<5;i++)for(let r=24;r>=0;r-=8)e[s]=this.chain_[i]>>r&255,++s;return e}}function Fv(t,e){const n=new Uv(t,e);return n.subscribe.bind(n)}class Uv{constructor(e,n){this.observers=[],this.unsubscribes=[],this.observerCount=0,this.task=Promise.resolve(),this.finalized=!1,this.onNoObservers=n,this.task.then(()=>{e(this)}).catch(s=>{this.error(s)})}next(e){this.forEachObserver(n=>{n.next(e)})}error(e){this.forEachObserver(n=>{n.error(e)}),this.close(e)}complete(){this.forEachObserver(e=>{e.complete()}),this.close()}subscribe(e,n,s){let i;if(e===void 0&&n===void 0&&s===void 0)throw new Error("Missing Observer.");Wv(e,["next","error","complete"])?i=e:i={next:e,error:n,complete:s},i.next===void 0&&(i.next=Eo),i.error===void 0&&(i.error=Eo),i.complete===void 0&&(i.complete=Eo);const r=this.unsubscribeOne.bind(this,this.observers.length);return this.finalized&&this.task.then(()=>{try{this.finalError?i.error(this.finalError):i.complete()}catch{}}),this.observers.push(i),r}unsubscribeOne(e){this.observers===void 0||this.observers[e]===void 0||(delete this.observers[e],this.observerCount-=1,this.observerCount===0&&this.onNoObservers!==void 0&&this.onNoObservers(this))}forEachObserver(e){if(!this.finalized)for(let n=0;n<this.observers.length;n++)this.sendOne(n,e)}sendOne(e,n){this.task.then(()=>{if(this.observers!==void 0&&this.observers[e]!==void 0)try{n(this.observers[e])}catch(s){typeof console<"u"&&console.error&&console.error(s)}})}close(e){this.finalized||(this.finalized=!0,e!==void 0&&(this.finalError=e),this.task.then(()=>{this.observers=void 0,this.onNoObservers=void 0}))}}function Wv(t,e){if(typeof t!="object"||t===null)return!1;for(const n of e)if(n in t&&typeof t[n]=="function")return!0;return!1}function Eo(){}function Wa(t,e){return`${t} failed: ${e} argument `}/**
 * @license
 * Copyright 2017 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */const Hv=function(t){const e=[];let n=0;for(let s=0;s<t.length;s++){let i=t.charCodeAt(s);if(i>=55296&&i<=56319){const r=i-55296;s++,C(s<t.length,"Surrogate pair missing trail surrogate.");const o=t.charCodeAt(s)-56320;i=65536+(r<<10)+o}i<128?e[n++]=i:i<2048?(e[n++]=i>>6|192,e[n++]=i&63|128):i<65536?(e[n++]=i>>12|224,e[n++]=i>>6&63|128,e[n++]=i&63|128):(e[n++]=i>>18|240,e[n++]=i>>12&63|128,e[n++]=i>>6&63|128,e[n++]=i&63|128)}return e},Kr=function(t){let e=0;for(let n=0;n<t.length;n++){const s=t.charCodeAt(n);s<128?e++:s<2048?e+=2:s>=55296&&s<=56319?(e+=4,n++):e+=3}return e};/**
 * @license
 * Copyright 2021 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */function Ue(t){return t&&t._delegate?t._delegate:t}class Ln{constructor(e,n,s){this.name=e,this.instanceFactory=n,this.type=s,this.multipleInstances=!1,this.serviceProps={},this.instantiationMode="LAZY",this.onInstanceCreated=null}setInstantiationMode(e){return this.instantiationMode=e,this}setMultipleInstances(e){return this.multipleInstances=e,this}setServiceProps(e){return this.serviceProps=e,this}setInstanceCreatedCallback(e){return this.onInstanceCreated=e,this}}/**
 * @license
 * Copyright 2019 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */const Tn="[DEFAULT]";/**
 * @license
 * Copyright 2019 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */class Vv{constructor(e,n){this.name=e,this.container=n,this.component=null,this.instances=new Map,this.instancesDeferred=new Map,this.instancesOptions=new Map,this.onInitCallbacks=new Map}get(e){const n=this.normalizeInstanceIdentifier(e);if(!this.instancesDeferred.has(n)){const s=new Gr;if(this.instancesDeferred.set(n,s),this.isInitialized(n)||this.shouldAutoInitialize())try{const i=this.getOrInitializeService({instanceIdentifier:n});i&&s.resolve(i)}catch{}}return this.instancesDeferred.get(n).promise}getImmediate(e){var n;const s=this.normalizeInstanceIdentifier(e==null?void 0:e.identifier),i=(n=e==null?void 0:e.optional)!==null&&n!==void 0?n:!1;if(this.isInitialized(s)||this.shouldAutoInitialize())try{return this.getOrInitializeService({instanceIdentifier:s})}catch(r){if(i)return null;throw r}else{if(i)return null;throw Error(`Service ${this.name} is not available`)}}getComponent(){return this.component}setComponent(e){if(e.name!==this.name)throw Error(`Mismatching Component ${e.name} for Provider ${this.name}.`);if(this.component)throw Error(`Component for ${this.name} has already been provided`);if(this.component=e,!!this.shouldAutoInitialize()){if($v(e))try{this.getOrInitializeService({instanceIdentifier:Tn})}catch{}for(const[n,s]of this.instancesDeferred.entries()){const i=this.normalizeInstanceIdentifier(n);try{const r=this.getOrInitializeService({instanceIdentifier:i});s.resolve(r)}catch{}}}}clearInstance(e=Tn){this.instancesDeferred.delete(e),this.instancesOptions.delete(e),this.instances.delete(e)}async delete(){const e=Array.from(this.instances.values());await Promise.all([...e.filter(n=>"INTERNAL"in n).map(n=>n.INTERNAL.delete()),...e.filter(n=>"_delete"in n).map(n=>n._delete())])}isComponentSet(){return this.component!=null}isInitialized(e=Tn){return this.instances.has(e)}getOptions(e=Tn){return this.instancesOptions.get(e)||{}}initialize(e={}){const{options:n={}}=e,s=this.normalizeInstanceIdentifier(e.instanceIdentifier);if(this.isInitialized(s))throw Error(`${this.name}(${s}) has already been initialized`);if(!this.isComponentSet())throw Error(`Component ${this.name} has not been registered yet`);const i=this.getOrInitializeService({instanceIdentifier:s,options:n});for(const[r,o]of this.instancesDeferred.entries()){const a=this.normalizeInstanceIdentifier(r);s===a&&o.resolve(i)}return i}onInit(e,n){var s;const i=this.normalizeInstanceIdentifier(n),r=(s=this.onInitCallbacks.get(i))!==null&&s!==void 0?s:new Set;r.add(e),this.onInitCallbacks.set(i,r);const o=this.instances.get(i);return o&&e(o,i),()=>{r.delete(e)}}invokeOnInitCallbacks(e,n){const s=this.onInitCallbacks.get(n);if(s)for(const i of s)try{i(e,n)}catch{}}getOrInitializeService({instanceIdentifier:e,options:n={}}){let s=this.instances.get(e);if(!s&&this.component&&(s=this.component.instanceFactory(this.container,{instanceIdentifier:Bv(e),options:n}),this.instances.set(e,s),this.instancesOptions.set(e,n),this.invokeOnInitCallbacks(s,e),this.component.onInstanceCreated))try{this.component.onInstanceCreated(this.container,e,s)}catch{}return s||null}normalizeInstanceIdentifier(e=Tn){return this.component?this.component.multipleInstances?e:Tn:e}shouldAutoInitialize(){return!!this.component&&this.component.instantiationMode!=="EXPLICIT"}}function Bv(t){return t===Tn?void 0:t}function $v(t){return t.instantiationMode==="EAGER"}/**
 * @license
 * Copyright 2019 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */class jv{constructor(e){this.name=e,this.providers=new Map}addComponent(e){const n=this.getProvider(e.name);if(n.isComponentSet())throw new Error(`Component ${e.name} has already been registered with ${this.name}`);n.setComponent(e)}addOrOverwriteComponent(e){this.getProvider(e.name).isComponentSet()&&this.providers.delete(e.name),this.addComponent(e)}getProvider(e){if(this.providers.has(e))return this.providers.get(e);const n=new Vv(e,this);return this.providers.set(e,n),n}getProviders(){return Array.from(this.providers.values())}}/**
 * @license
 * Copyright 2017 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */var re;(function(t){t[t.DEBUG=0]="DEBUG",t[t.VERBOSE=1]="VERBOSE",t[t.INFO=2]="INFO",t[t.WARN=3]="WARN",t[t.ERROR=4]="ERROR",t[t.SILENT=5]="SILENT"})(re||(re={}));const Gv={debug:re.DEBUG,verbose:re.VERBOSE,info:re.INFO,warn:re.WARN,error:re.ERROR,silent:re.SILENT},Kv=re.INFO,qv={[re.DEBUG]:"log",[re.VERBOSE]:"log",[re.INFO]:"info",[re.WARN]:"warn",[re.ERROR]:"error"},zv=(t,e,...n)=>{if(e<t.logLevel)return;const s=new Date().toISOString(),i=qv[e];if(i)console[i](`[${s}]  ${t.name}:`,...n);else throw new Error(`Attempted to log a message with an invalid logType (value: ${e})`)};class Ha{constructor(e){this.name=e,this._logLevel=Kv,this._logHandler=zv,this._userLogHandler=null}get logLevel(){return this._logLevel}set logLevel(e){if(!(e in re))throw new TypeError(`Invalid value "${e}" assigned to \`logLevel\``);this._logLevel=e}setLogLevel(e){this._logLevel=typeof e=="string"?Gv[e]:e}get logHandler(){return this._logHandler}set logHandler(e){if(typeof e!="function")throw new TypeError("Value assigned to `logHandler` must be a function");this._logHandler=e}get userLogHandler(){return this._userLogHandler}set userLogHandler(e){this._userLogHandler=e}debug(...e){this._userLogHandler&&this._userLogHandler(this,re.DEBUG,...e),this._logHandler(this,re.DEBUG,...e)}log(...e){this._userLogHandler&&this._userLogHandler(this,re.VERBOSE,...e),this._logHandler(this,re.VERBOSE,...e)}info(...e){this._userLogHandler&&this._userLogHandler(this,re.INFO,...e),this._logHandler(this,re.INFO,...e)}warn(...e){this._userLogHandler&&this._userLogHandler(this,re.WARN,...e),this._logHandler(this,re.WARN,...e)}error(...e){this._userLogHandler&&this._userLogHandler(this,re.ERROR,...e),this._logHandler(this,re.ERROR,...e)}}const Yv=(t,e)=>e.some(n=>t instanceof n);let Mc,Lc;function Qv(){return Mc||(Mc=[IDBDatabase,IDBObjectStore,IDBIndex,IDBCursor,IDBTransaction])}function Jv(){return Lc||(Lc=[IDBCursor.prototype.advance,IDBCursor.prototype.continue,IDBCursor.prototype.continuePrimaryKey])}const Ad=new WeakMap,Qo=new WeakMap,Nd=new WeakMap,wo=new WeakMap,Va=new WeakMap;function Xv(t){const e=new Promise((n,s)=>{const i=()=>{t.removeEventListener("success",r),t.removeEventListener("error",o)},r=()=>{n(on(t.result)),i()},o=()=>{s(t.error),i()};t.addEventListener("success",r),t.addEventListener("error",o)});return e.then(n=>{n instanceof IDBCursor&&Ad.set(n,t)}).catch(()=>{}),Va.set(e,t),e}function Zv(t){if(Qo.has(t))return;const e=new Promise((n,s)=>{const i=()=>{t.removeEventListener("complete",r),t.removeEventListener("error",o),t.removeEventListener("abort",o)},r=()=>{n(),i()},o=()=>{s(t.error||new DOMException("AbortError","AbortError")),i()};t.addEventListener("complete",r),t.addEventListener("error",o),t.addEventListener("abort",o)});Qo.set(t,e)}let Jo={get(t,e,n){if(t instanceof IDBTransaction){if(e==="done")return Qo.get(t);if(e==="objectStoreNames")return t.objectStoreNames||Nd.get(t);if(e==="store")return n.objectStoreNames[1]?void 0:n.objectStore(n.objectStoreNames[0])}return on(t[e])},set(t,e,n){return t[e]=n,!0},has(t,e){return t instanceof IDBTransaction&&(e==="done"||e==="store")?!0:e in t}};function ey(t){Jo=t(Jo)}function ty(t){return t===IDBDatabase.prototype.transaction&&!("objectStoreNames"in IDBTransaction.prototype)?function(e,...n){const s=t.call(Io(this),e,...n);return Nd.set(s,e.sort?e.sort():[e]),on(s)}:Jv().includes(t)?function(...e){return t.apply(Io(this),e),on(Ad.get(this))}:function(...e){return on(t.apply(Io(this),e))}}function ny(t){return typeof t=="function"?ty(t):(t instanceof IDBTransaction&&Zv(t),Yv(t,Qv())?new Proxy(t,Jo):t)}function on(t){if(t instanceof IDBRequest)return Xv(t);if(wo.has(t))return wo.get(t);const e=ny(t);return e!==t&&(wo.set(t,e),Va.set(e,t)),e}const Io=t=>Va.get(t);function sy(t,e,{blocked:n,upgrade:s,blocking:i,terminated:r}={}){const o=indexedDB.open(t,e),a=on(o);return s&&o.addEventListener("upgradeneeded",l=>{s(on(o.result),l.oldVersion,l.newVersion,on(o.transaction),l)}),n&&o.addEventListener("blocked",l=>n(l.oldVersion,l.newVersion,l)),a.then(l=>{r&&l.addEventListener("close",()=>r()),i&&l.addEventListener("versionchange",c=>i(c.oldVersion,c.newVersion,c))}).catch(()=>{}),a}const iy=["get","getKey","getAll","getAllKeys","count"],ry=["put","add","delete","clear"],bo=new Map;function Fc(t,e){if(!(t instanceof IDBDatabase&&!(e in t)&&typeof e=="string"))return;if(bo.get(e))return bo.get(e);const n=e.replace(/FromIndex$/,""),s=e!==n,i=ry.includes(n);if(!(n in(s?IDBIndex:IDBObjectStore).prototype)||!(i||iy.includes(n)))return;const r=async function(o,...a){const l=this.transaction(o,i?"readwrite":"readonly");let c=l.store;return s&&(c=c.index(a.shift())),(await Promise.all([c[n](...a),i&&l.done]))[0]};return bo.set(e,r),r}ey(t=>({...t,get:(e,n,s)=>Fc(e,n)||t.get(e,n,s),has:(e,n)=>!!Fc(e,n)||t.has(e,n)}));/**
 * @license
 * Copyright 2019 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */class oy{constructor(e){this.container=e}getPlatformInfoString(){return this.container.getProviders().map(n=>{if(ay(n)){const s=n.getImmediate();return`${s.library}/${s.version}`}else return null}).filter(n=>n).join(" ")}}function ay(t){const e=t.getComponent();return(e==null?void 0:e.type)==="VERSION"}const Xo="@firebase/app",Uc="0.10.13";/**
 * @license
 * Copyright 2019 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */const Ft=new Ha("@firebase/app"),ly="@firebase/app-compat",cy="@firebase/analytics-compat",uy="@firebase/analytics",hy="@firebase/app-check-compat",dy="@firebase/app-check",fy="@firebase/auth",py="@firebase/auth-compat",_y="@firebase/database",gy="@firebase/data-connect",my="@firebase/database-compat",vy="@firebase/functions",yy="@firebase/functions-compat",Ey="@firebase/installations",wy="@firebase/installations-compat",Iy="@firebase/messaging",by="@firebase/messaging-compat",Cy="@firebase/performance",Sy="@firebase/performance-compat",Ty="@firebase/remote-config",Ry="@firebase/remote-config-compat",Py="@firebase/storage",Ay="@firebase/storage-compat",Ny="@firebase/firestore",ky="@firebase/vertexai-preview",Oy="@firebase/firestore-compat",xy="firebase",Dy="10.14.1";/**
 * @license
 * Copyright 2019 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */const Zo="[DEFAULT]",My={[Xo]:"fire-core",[ly]:"fire-core-compat",[uy]:"fire-analytics",[cy]:"fire-analytics-compat",[dy]:"fire-app-check",[hy]:"fire-app-check-compat",[fy]:"fire-auth",[py]:"fire-auth-compat",[_y]:"fire-rtdb",[gy]:"fire-data-connect",[my]:"fire-rtdb-compat",[vy]:"fire-fn",[yy]:"fire-fn-compat",[Ey]:"fire-iid",[wy]:"fire-iid-compat",[Iy]:"fire-fcm",[by]:"fire-fcm-compat",[Cy]:"fire-perf",[Sy]:"fire-perf-compat",[Ty]:"fire-rc",[Ry]:"fire-rc-compat",[Py]:"fire-gcs",[Ay]:"fire-gcs-compat",[Ny]:"fire-fst",[Oy]:"fire-fst-compat",[ky]:"fire-vertex","fire-js":"fire-js",[xy]:"fire-js-all"};/**
 * @license
 * Copyright 2019 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */const cr=new Map,Ly=new Map,ea=new Map;function Wc(t,e){try{t.container.addComponent(e)}catch(n){Ft.debug(`Component ${e.name} failed to register with FirebaseApp ${t.name}`,n)}}function fs(t){const e=t.name;if(ea.has(e))return Ft.debug(`There were multiple attempts to register component ${e}.`),!1;ea.set(e,t);for(const n of cr.values())Wc(n,t);for(const n of Ly.values())Wc(n,t);return!0}function Ba(t,e){const n=t.container.getProvider("heartbeat").getImmediate({optional:!0});return n&&n.triggerHeartbeat(),t.container.getProvider(e)}function it(t){return t.settings!==void 0}/**
 * @license
 * Copyright 2019 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */const Fy={"no-app":"No Firebase App '{$appName}' has been created - call initializeApp() first","bad-app-name":"Illegal App name: '{$appName}'","duplicate-app":"Firebase App named '{$appName}' already exists with different options or config","app-deleted":"Firebase App named '{$appName}' already deleted","server-app-deleted":"Firebase Server App has been deleted","no-options":"Need to provide options, when not being deployed to hosting via source.","invalid-app-argument":"firebase.{$appName}() takes either no argument or a Firebase App instance.","invalid-log-argument":"First argument to `onLog` must be null or a function.","idb-open":"Error thrown when opening IndexedDB. Original error: {$originalErrorMessage}.","idb-get":"Error thrown when reading from IndexedDB. Original error: {$originalErrorMessage}.","idb-set":"Error thrown when writing to IndexedDB. Original error: {$originalErrorMessage}.","idb-delete":"Error thrown when deleting from IndexedDB. Original error: {$originalErrorMessage}.","finalization-registry-not-supported":"FirebaseServerApp deleteOnDeref field defined but the JS runtime does not support FinalizationRegistry.","invalid-server-app-environment":"FirebaseServerApp is not for use in browser environments."},an=new Ei("app","Firebase",Fy);/**
 * @license
 * Copyright 2019 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */class Uy{constructor(e,n,s){this._isDeleted=!1,this._options=Object.assign({},e),this._config=Object.assign({},n),this._name=n.name,this._automaticDataCollectionEnabled=n.automaticDataCollectionEnabled,this._container=s,this.container.addComponent(new Ln("app",()=>this,"PUBLIC"))}get automaticDataCollectionEnabled(){return this.checkDestroyed(),this._automaticDataCollectionEnabled}set automaticDataCollectionEnabled(e){this.checkDestroyed(),this._automaticDataCollectionEnabled=e}get name(){return this.checkDestroyed(),this._name}get options(){return this.checkDestroyed(),this._options}get config(){return this.checkDestroyed(),this._config}get container(){return this._container}get isDeleted(){return this._isDeleted}set isDeleted(e){this._isDeleted=e}checkDestroyed(){if(this.isDeleted)throw an.create("app-deleted",{appName:this._name})}}/**
 * @license
 * Copyright 2019 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */const ys=Dy;function kd(t,e={}){let n=t;typeof e!="object"&&(e={name:e});const s=Object.assign({name:Zo,automaticDataCollectionEnabled:!1},e),i=s.name;if(typeof i!="string"||!i)throw an.create("bad-app-name",{appName:String(i)});if(n||(n=Sd()),!n)throw an.create("no-options");const r=cr.get(i);if(r){if(lr(n,r.options)&&lr(s,r.config))return r;throw an.create("duplicate-app",{appName:i})}const o=new jv(i);for(const l of ea.values())o.addComponent(l);const a=new Uy(n,s,o);return cr.set(i,a),a}function Od(t=Zo){const e=cr.get(t);if(!e&&t===Zo&&Sd())return kd();if(!e)throw an.create("no-app",{appName:t});return e}function ln(t,e,n){var s;let i=(s=My[t])!==null&&s!==void 0?s:t;n&&(i+=`-${n}`);const r=i.match(/\s|\//),o=e.match(/\s|\//);if(r||o){const a=[`Unable to register library "${i}" with version "${e}":`];r&&a.push(`library name "${i}" contains illegal characters (whitespace or "/")`),r&&o&&a.push("and"),o&&a.push(`version name "${e}" contains illegal characters (whitespace or "/")`),Ft.warn(a.join(" "));return}fs(new Ln(`${i}-version`,()=>({library:i,version:e}),"VERSION"))}/**
 * @license
 * Copyright 2021 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */const Wy="firebase-heartbeat-database",Hy=1,ii="firebase-heartbeat-store";let Co=null;function xd(){return Co||(Co=sy(Wy,Hy,{upgrade:(t,e)=>{switch(e){case 0:try{t.createObjectStore(ii)}catch(n){console.warn(n)}}}}).catch(t=>{throw an.create("idb-open",{originalErrorMessage:t.message})})),Co}async function Vy(t){try{const n=(await xd()).transaction(ii),s=await n.objectStore(ii).get(Dd(t));return await n.done,s}catch(e){if(e instanceof gn)Ft.warn(e.message);else{const n=an.create("idb-get",{originalErrorMessage:e==null?void 0:e.message});Ft.warn(n.message)}}}async function Hc(t,e){try{const s=(await xd()).transaction(ii,"readwrite");await s.objectStore(ii).put(e,Dd(t)),await s.done}catch(n){if(n instanceof gn)Ft.warn(n.message);else{const s=an.create("idb-set",{originalErrorMessage:n==null?void 0:n.message});Ft.warn(s.message)}}}function Dd(t){return`${t.name}!${t.options.appId}`}/**
 * @license
 * Copyright 2021 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */const By=1024,$y=30*24*60*60*1e3;class jy{constructor(e){this.container=e,this._heartbeatsCache=null;const n=this.container.getProvider("app").getImmediate();this._storage=new Ky(n),this._heartbeatsCachePromise=this._storage.read().then(s=>(this._heartbeatsCache=s,s))}async triggerHeartbeat(){var e,n;try{const i=this.container.getProvider("platform-logger").getImmediate().getPlatformInfoString(),r=Vc();return((e=this._heartbeatsCache)===null||e===void 0?void 0:e.heartbeats)==null&&(this._heartbeatsCache=await this._heartbeatsCachePromise,((n=this._heartbeatsCache)===null||n===void 0?void 0:n.heartbeats)==null)||this._heartbeatsCache.lastSentHeartbeatDate===r||this._heartbeatsCache.heartbeats.some(o=>o.date===r)?void 0:(this._heartbeatsCache.heartbeats.push({date:r,agent:i}),this._heartbeatsCache.heartbeats=this._heartbeatsCache.heartbeats.filter(o=>{const a=new Date(o.date).valueOf();return Date.now()-a<=$y}),this._storage.overwrite(this._heartbeatsCache))}catch(s){Ft.warn(s)}}async getHeartbeatsHeader(){var e;try{if(this._heartbeatsCache===null&&await this._heartbeatsCachePromise,((e=this._heartbeatsCache)===null||e===void 0?void 0:e.heartbeats)==null||this._heartbeatsCache.heartbeats.length===0)return"";const n=Vc(),{heartbeatsToSend:s,unsentEntries:i}=Gy(this._heartbeatsCache.heartbeats),r=rr(JSON.stringify({version:2,heartbeats:s}));return this._heartbeatsCache.lastSentHeartbeatDate=n,i.length>0?(this._heartbeatsCache.heartbeats=i,await this._storage.overwrite(this._heartbeatsCache)):(this._heartbeatsCache.heartbeats=[],this._storage.overwrite(this._heartbeatsCache)),r}catch(n){return Ft.warn(n),""}}}function Vc(){return new Date().toISOString().substring(0,10)}function Gy(t,e=By){const n=[];let s=t.slice();for(const i of t){const r=n.find(o=>o.agent===i.agent);if(r){if(r.dates.push(i.date),Bc(n)>e){r.dates.pop();break}}else if(n.push({agent:i.agent,dates:[i.date]}),Bc(n)>e){n.pop();break}s=s.slice(1)}return{heartbeatsToSend:n,unsentEntries:s}}class Ky{constructor(e){this.app=e,this._canUseIndexedDBPromise=this.runIndexedDBEnvironmentCheck()}async runIndexedDBEnvironmentCheck(){return Av()?Nv().then(()=>!0).catch(()=>!1):!1}async read(){if(await this._canUseIndexedDBPromise){const n=await Vy(this.app);return n!=null&&n.heartbeats?n:{heartbeats:[]}}else return{heartbeats:[]}}async overwrite(e){var n;if(await this._canUseIndexedDBPromise){const i=await this.read();return Hc(this.app,{lastSentHeartbeatDate:(n=e.lastSentHeartbeatDate)!==null&&n!==void 0?n:i.lastSentHeartbeatDate,heartbeats:e.heartbeats})}else return}async add(e){var n;if(await this._canUseIndexedDBPromise){const i=await this.read();return Hc(this.app,{lastSentHeartbeatDate:(n=e.lastSentHeartbeatDate)!==null&&n!==void 0?n:i.lastSentHeartbeatDate,heartbeats:[...i.heartbeats,...e.heartbeats]})}else return}}function Bc(t){return rr(JSON.stringify({version:2,heartbeats:t})).length}/**
 * @license
 * Copyright 2019 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */function qy(t){fs(new Ln("platform-logger",e=>new oy(e),"PRIVATE")),fs(new Ln("heartbeat",e=>new jy(e),"PRIVATE")),ln(Xo,Uc,t),ln(Xo,Uc,"esm2017"),ln("fire-js","")}qy("");var zy="firebase",Yy="10.14.1";/**
 * @license
 * Copyright 2020 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */ln(zy,Yy,"app");function $a(t,e){var n={};for(var s in t)Object.prototype.hasOwnProperty.call(t,s)&&e.indexOf(s)<0&&(n[s]=t[s]);if(t!=null&&typeof Object.getOwnPropertySymbols=="function")for(var i=0,s=Object.getOwnPropertySymbols(t);i<s.length;i++)e.indexOf(s[i])<0&&Object.prototype.propertyIsEnumerable.call(t,s[i])&&(n[s[i]]=t[s[i]]);return n}function Md(){return{"dependent-sdk-initialized-before-auth":"Another Firebase SDK was initialized and is trying to use Auth before Auth is initialized. Please be sure to call `initializeAuth` or `getAuth` before starting any other Firebase SDK."}}const Qy=Md,Ld=new Ei("auth","Firebase",Md());/**
 * @license
 * Copyright 2020 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */const ur=new Ha("@firebase/auth");function Jy(t,...e){ur.logLevel<=re.WARN&&ur.warn(`Auth (${ys}): ${t}`,...e)}function $i(t,...e){ur.logLevel<=re.ERROR&&ur.error(`Auth (${ys}): ${t}`,...e)}/**
 * @license
 * Copyright 2020 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */function nt(t,...e){throw Ga(t,...e)}function lt(t,...e){return Ga(t,...e)}function ja(t,e,n){const s=Object.assign(Object.assign({},Qy()),{[e]:n});return new Ei("auth","Firebase",s).create(e,{appName:t.name})}function Mt(t){return ja(t,"operation-not-supported-in-this-environment","Operations that alter the current user are not supported in conjunction with FirebaseServerApp")}function Xy(t,e,n){const s=n;if(!(e instanceof s))throw s.name!==e.constructor.name&&nt(t,"argument-error"),ja(t,"argument-error",`Type of ${e.constructor.name} does not match expected instance.Did you pass a reference from a different Auth SDK?`)}function Ga(t,...e){if(typeof t!="string"){const n=e[0],s=[...e.slice(1)];return s[0]&&(s[0].appName=t.name),t._errorFactory.create(n,...s)}return Ld.create(t,...e)}function V(t,e,...n){if(!t)throw Ga(e,...n)}function kt(t){const e="INTERNAL ASSERTION FAILED: "+t;throw $i(e),new Error(e)}function Ut(t,e){t||kt(e)}/**
 * @license
 * Copyright 2020 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */function ta(){var t;return typeof self<"u"&&((t=self.location)===null||t===void 0?void 0:t.href)||""}function Zy(){return $c()==="http:"||$c()==="https:"}function $c(){var t;return typeof self<"u"&&((t=self.location)===null||t===void 0?void 0:t.protocol)||null}/**
 * @license
 * Copyright 2020 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */function eE(){return typeof navigator<"u"&&navigator&&"onLine"in navigator&&typeof navigator.onLine=="boolean"&&(Zy()||Tv()||"connection"in navigator)?navigator.onLine:!0}function tE(){if(typeof navigator>"u")return null;const t=navigator;return t.languages&&t.languages[0]||t.language||null}/**
 * @license
 * Copyright 2020 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */class wi{constructor(e,n){this.shortDelay=e,this.longDelay=n,Ut(n>e,"Short delay should be less than long delay!"),this.isMobile=Ua()||Rd()}get(){return eE()?this.isMobile?this.longDelay:this.shortDelay:Math.min(5e3,this.shortDelay)}}/**
 * @license
 * Copyright 2020 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */function Ka(t,e){Ut(t.emulator,"Emulator should always be set here");const{url:n}=t.emulator;return e?`${n}${e.startsWith("/")?e.slice(1):e}`:n}/**
 * @license
 * Copyright 2020 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */class Fd{static initialize(e,n,s){this.fetchImpl=e,n&&(this.headersImpl=n),s&&(this.responseImpl=s)}static fetch(){if(this.fetchImpl)return this.fetchImpl;if(typeof self<"u"&&"fetch"in self)return self.fetch;if(typeof globalThis<"u"&&globalThis.fetch)return globalThis.fetch;if(typeof fetch<"u")return fetch;kt("Could not find fetch implementation, make sure you call FetchProvider.initialize() with an appropriate polyfill")}static headers(){if(this.headersImpl)return this.headersImpl;if(typeof self<"u"&&"Headers"in self)return self.Headers;if(typeof globalThis<"u"&&globalThis.Headers)return globalThis.Headers;if(typeof Headers<"u")return Headers;kt("Could not find Headers implementation, make sure you call FetchProvider.initialize() with an appropriate polyfill")}static response(){if(this.responseImpl)return this.responseImpl;if(typeof self<"u"&&"Response"in self)return self.Response;if(typeof globalThis<"u"&&globalThis.Response)return globalThis.Response;if(typeof Response<"u")return Response;kt("Could not find Response implementation, make sure you call FetchProvider.initialize() with an appropriate polyfill")}}/**
 * @license
 * Copyright 2020 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */const nE={CREDENTIAL_MISMATCH:"custom-token-mismatch",MISSING_CUSTOM_TOKEN:"internal-error",INVALID_IDENTIFIER:"invalid-email",MISSING_CONTINUE_URI:"internal-error",INVALID_PASSWORD:"wrong-password",MISSING_PASSWORD:"missing-password",INVALID_LOGIN_CREDENTIALS:"invalid-credential",EMAIL_EXISTS:"email-already-in-use",PASSWORD_LOGIN_DISABLED:"operation-not-allowed",INVALID_IDP_RESPONSE:"invalid-credential",INVALID_PENDING_TOKEN:"invalid-credential",FEDERATED_USER_ID_ALREADY_LINKED:"credential-already-in-use",MISSING_REQ_TYPE:"internal-error",EMAIL_NOT_FOUND:"user-not-found",RESET_PASSWORD_EXCEED_LIMIT:"too-many-requests",EXPIRED_OOB_CODE:"expired-action-code",INVALID_OOB_CODE:"invalid-action-code",MISSING_OOB_CODE:"internal-error",CREDENTIAL_TOO_OLD_LOGIN_AGAIN:"requires-recent-login",INVALID_ID_TOKEN:"invalid-user-token",TOKEN_EXPIRED:"user-token-expired",USER_NOT_FOUND:"user-token-expired",TOO_MANY_ATTEMPTS_TRY_LATER:"too-many-requests",PASSWORD_DOES_NOT_MEET_REQUIREMENTS:"password-does-not-meet-requirements",INVALID_CODE:"invalid-verification-code",INVALID_SESSION_INFO:"invalid-verification-id",INVALID_TEMPORARY_PROOF:"invalid-credential",MISSING_SESSION_INFO:"missing-verification-id",SESSION_EXPIRED:"code-expired",MISSING_ANDROID_PACKAGE_NAME:"missing-android-pkg-name",UNAUTHORIZED_DOMAIN:"unauthorized-continue-uri",INVALID_OAUTH_CLIENT_ID:"invalid-oauth-client-id",ADMIN_ONLY_OPERATION:"admin-restricted-operation",INVALID_MFA_PENDING_CREDENTIAL:"invalid-multi-factor-session",MFA_ENROLLMENT_NOT_FOUND:"multi-factor-info-not-found",MISSING_MFA_ENROLLMENT_ID:"missing-multi-factor-info",MISSING_MFA_PENDING_CREDENTIAL:"missing-multi-factor-session",SECOND_FACTOR_EXISTS:"second-factor-already-in-use",SECOND_FACTOR_LIMIT_EXCEEDED:"maximum-second-factor-count-exceeded",BLOCKING_FUNCTION_ERROR_RESPONSE:"internal-error",RECAPTCHA_NOT_ENABLED:"recaptcha-not-enabled",MISSING_RECAPTCHA_TOKEN:"missing-recaptcha-token",INVALID_RECAPTCHA_TOKEN:"invalid-recaptcha-token",INVALID_RECAPTCHA_ACTION:"invalid-recaptcha-action",MISSING_CLIENT_TYPE:"missing-client-type",MISSING_RECAPTCHA_VERSION:"missing-recaptcha-version",INVALID_RECAPTCHA_VERSION:"invalid-recaptcha-version",INVALID_REQ_TYPE:"invalid-req-type"};/**
 * @license
 * Copyright 2020 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */const sE=new wi(3e4,6e4);function mn(t,e){return t.tenantId&&!e.tenantId?Object.assign(Object.assign({},e),{tenantId:t.tenantId}):e}async function vn(t,e,n,s,i={}){return Ud(t,i,async()=>{let r={},o={};s&&(e==="GET"?o=s:r={body:JSON.stringify(s)});const a=vs(Object.assign({key:t.config.apiKey},o)).slice(1),l=await t._getAdditionalHeaders();l["Content-Type"]="application/json",t.languageCode&&(l["X-Firebase-Locale"]=t.languageCode);const c=Object.assign({method:e,headers:l},r);return Sv()||(c.referrerPolicy="no-referrer"),Fd.fetch()(Wd(t,t.config.apiHost,n,a),c)})}async function Ud(t,e,n){t._canInitEmulator=!1;const s=Object.assign(Object.assign({},nE),e);try{const i=new rE(t),r=await Promise.race([n(),i.promise]);i.clearNetworkTimeout();const o=await r.json();if("needConfirmation"in o)throw Mi(t,"account-exists-with-different-credential",o);if(r.ok&&!("errorMessage"in o))return o;{const a=r.ok?o.errorMessage:o.error.message,[l,c]=a.split(" : ");if(l==="FEDERATED_USER_ID_ALREADY_LINKED")throw Mi(t,"credential-already-in-use",o);if(l==="EMAIL_EXISTS")throw Mi(t,"email-already-in-use",o);if(l==="USER_DISABLED")throw Mi(t,"user-disabled",o);const h=s[l]||l.toLowerCase().replace(/[_\s]+/g,"-");if(c)throw ja(t,h,c);nt(t,h)}}catch(i){if(i instanceof gn)throw i;nt(t,"network-request-failed",{message:String(i)})}}async function Ii(t,e,n,s,i={}){const r=await vn(t,e,n,s,i);return"mfaPendingCredential"in r&&nt(t,"multi-factor-auth-required",{_serverResponse:r}),r}function Wd(t,e,n,s){const i=`${e}${n}?${s}`;return t.config.emulator?Ka(t.config,i):`${t.config.apiScheme}://${i}`}function iE(t){switch(t){case"ENFORCE":return"ENFORCE";case"AUDIT":return"AUDIT";case"OFF":return"OFF";default:return"ENFORCEMENT_STATE_UNSPECIFIED"}}class rE{constructor(e){this.auth=e,this.timer=null,this.promise=new Promise((n,s)=>{this.timer=setTimeout(()=>s(lt(this.auth,"network-request-failed")),sE.get())})}clearNetworkTimeout(){clearTimeout(this.timer)}}function Mi(t,e,n){const s={appName:t.name};n.email&&(s.email=n.email),n.phoneNumber&&(s.phoneNumber=n.phoneNumber);const i=lt(t,e,s);return i.customData._tokenResponse=n,i}function jc(t){return t!==void 0&&t.enterprise!==void 0}class oE{constructor(e){if(this.siteKey="",this.recaptchaEnforcementState=[],e.recaptchaKey===void 0)throw new Error("recaptchaKey undefined");this.siteKey=e.recaptchaKey.split("/")[3],this.recaptchaEnforcementState=e.recaptchaEnforcementState}getProviderEnforcementState(e){if(!this.recaptchaEnforcementState||this.recaptchaEnforcementState.length===0)return null;for(const n of this.recaptchaEnforcementState)if(n.provider&&n.provider===e)return iE(n.enforcementState);return null}isProviderEnabled(e){return this.getProviderEnforcementState(e)==="ENFORCE"||this.getProviderEnforcementState(e)==="AUDIT"}}async function aE(t,e){return vn(t,"GET","/v2/recaptchaConfig",mn(t,e))}/**
 * @license
 * Copyright 2020 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */async function lE(t,e){return vn(t,"POST","/v1/accounts:delete",e)}async function Hd(t,e){return vn(t,"POST","/v1/accounts:lookup",e)}/**
 * @license
 * Copyright 2020 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */function js(t){if(t)try{const e=new Date(Number(t));if(!isNaN(e.getTime()))return e.toUTCString()}catch{}}async function cE(t,e=!1){const n=Ue(t),s=await n.getIdToken(e),i=qa(s);V(i&&i.exp&&i.auth_time&&i.iat,n.auth,"internal-error");const r=typeof i.firebase=="object"?i.firebase:void 0,o=r==null?void 0:r.sign_in_provider;return{claims:i,token:s,authTime:js(So(i.auth_time)),issuedAtTime:js(So(i.iat)),expirationTime:js(So(i.exp)),signInProvider:o||null,signInSecondFactor:(r==null?void 0:r.sign_in_second_factor)||null}}function So(t){return Number(t)*1e3}function qa(t){const[e,n,s]=t.split(".");if(e===void 0||n===void 0||s===void 0)return $i("JWT malformed, contained fewer than 3 sections"),null;try{const i=or(n);return i?JSON.parse(i):($i("Failed to decode base64 JWT payload"),null)}catch(i){return $i("Caught error parsing JWT payload as JSON",i==null?void 0:i.toString()),null}}function Gc(t){const e=qa(t);return V(e,"internal-error"),V(typeof e.exp<"u","internal-error"),V(typeof e.iat<"u","internal-error"),Number(e.exp)-Number(e.iat)}/**
 * @license
 * Copyright 2020 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */async function ri(t,e,n=!1){if(n)return e;try{return await e}catch(s){throw s instanceof gn&&uE(s)&&t.auth.currentUser===t&&await t.auth.signOut(),s}}function uE({code:t}){return t==="auth/user-disabled"||t==="auth/user-token-expired"}/**
 * @license
 * Copyright 2020 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */class hE{constructor(e){this.user=e,this.isRunning=!1,this.timerId=null,this.errorBackoff=3e4}_start(){this.isRunning||(this.isRunning=!0,this.schedule())}_stop(){this.isRunning&&(this.isRunning=!1,this.timerId!==null&&clearTimeout(this.timerId))}getInterval(e){var n;if(e){const s=this.errorBackoff;return this.errorBackoff=Math.min(this.errorBackoff*2,96e4),s}else{this.errorBackoff=3e4;const i=((n=this.user.stsTokenManager.expirationTime)!==null&&n!==void 0?n:0)-Date.now()-3e5;return Math.max(0,i)}}schedule(e=!1){if(!this.isRunning)return;const n=this.getInterval(e);this.timerId=setTimeout(async()=>{await this.iteration()},n)}async iteration(){try{await this.user.getIdToken(!0)}catch(e){(e==null?void 0:e.code)==="auth/network-request-failed"&&this.schedule(!0);return}this.schedule()}}/**
 * @license
 * Copyright 2020 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */class na{constructor(e,n){this.createdAt=e,this.lastLoginAt=n,this._initializeTime()}_initializeTime(){this.lastSignInTime=js(this.lastLoginAt),this.creationTime=js(this.createdAt)}_copy(e){this.createdAt=e.createdAt,this.lastLoginAt=e.lastLoginAt,this._initializeTime()}toJSON(){return{createdAt:this.createdAt,lastLoginAt:this.lastLoginAt}}}/**
 * @license
 * Copyright 2019 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */async function hr(t){var e;const n=t.auth,s=await t.getIdToken(),i=await ri(t,Hd(n,{idToken:s}));V(i==null?void 0:i.users.length,n,"internal-error");const r=i.users[0];t._notifyReloadListener(r);const o=!((e=r.providerUserInfo)===null||e===void 0)&&e.length?Vd(r.providerUserInfo):[],a=fE(t.providerData,o),l=t.isAnonymous,c=!(t.email&&r.passwordHash)&&!(a!=null&&a.length),h=l?c:!1,u={uid:r.localId,displayName:r.displayName||null,photoURL:r.photoUrl||null,email:r.email||null,emailVerified:r.emailVerified||!1,phoneNumber:r.phoneNumber||null,tenantId:r.tenantId||null,providerData:a,metadata:new na(r.createdAt,r.lastLoginAt),isAnonymous:h};Object.assign(t,u)}async function dE(t){const e=Ue(t);await hr(e),await e.auth._persistUserIfCurrent(e),e.auth._notifyListenersIfCurrent(e)}function fE(t,e){return[...t.filter(s=>!e.some(i=>i.providerId===s.providerId)),...e]}function Vd(t){return t.map(e=>{var{providerId:n}=e,s=$a(e,["providerId"]);return{providerId:n,uid:s.rawId||"",displayName:s.displayName||null,email:s.email||null,phoneNumber:s.phoneNumber||null,photoURL:s.photoUrl||null}})}/**
 * @license
 * Copyright 2020 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */async function pE(t,e){const n=await Ud(t,{},async()=>{const s=vs({grant_type:"refresh_token",refresh_token:e}).slice(1),{tokenApiHost:i,apiKey:r}=t.config,o=Wd(t,i,"/v1/token",`key=${r}`),a=await t._getAdditionalHeaders();return a["Content-Type"]="application/x-www-form-urlencoded",Fd.fetch()(o,{method:"POST",headers:a,body:s})});return{accessToken:n.access_token,expiresIn:n.expires_in,refreshToken:n.refresh_token}}async function _E(t,e){return vn(t,"POST","/v2/accounts:revokeToken",mn(t,e))}/**
 * @license
 * Copyright 2020 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */class is{constructor(){this.refreshToken=null,this.accessToken=null,this.expirationTime=null}get isExpired(){return!this.expirationTime||Date.now()>this.expirationTime-3e4}updateFromServerResponse(e){V(e.idToken,"internal-error"),V(typeof e.idToken<"u","internal-error"),V(typeof e.refreshToken<"u","internal-error");const n="expiresIn"in e&&typeof e.expiresIn<"u"?Number(e.expiresIn):Gc(e.idToken);this.updateTokensAndExpiration(e.idToken,e.refreshToken,n)}updateFromIdToken(e){V(e.length!==0,"internal-error");const n=Gc(e);this.updateTokensAndExpiration(e,null,n)}async getToken(e,n=!1){return!n&&this.accessToken&&!this.isExpired?this.accessToken:(V(this.refreshToken,e,"user-token-expired"),this.refreshToken?(await this.refresh(e,this.refreshToken),this.accessToken):null)}clearRefreshToken(){this.refreshToken=null}async refresh(e,n){const{accessToken:s,refreshToken:i,expiresIn:r}=await pE(e,n);this.updateTokensAndExpiration(s,i,Number(r))}updateTokensAndExpiration(e,n,s){this.refreshToken=n||null,this.accessToken=e||null,this.expirationTime=Date.now()+s*1e3}static fromJSON(e,n){const{refreshToken:s,accessToken:i,expirationTime:r}=n,o=new is;return s&&(V(typeof s=="string","internal-error",{appName:e}),o.refreshToken=s),i&&(V(typeof i=="string","internal-error",{appName:e}),o.accessToken=i),r&&(V(typeof r=="number","internal-error",{appName:e}),o.expirationTime=r),o}toJSON(){return{refreshToken:this.refreshToken,accessToken:this.accessToken,expirationTime:this.expirationTime}}_assign(e){this.accessToken=e.accessToken,this.refreshToken=e.refreshToken,this.expirationTime=e.expirationTime}_clone(){return Object.assign(new is,this.toJSON())}_performRefresh(){return kt("not implemented")}}/**
 * @license
 * Copyright 2020 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */function Kt(t,e){V(typeof t=="string"||typeof t>"u","internal-error",{appName:e})}class Ot{constructor(e){var{uid:n,auth:s,stsTokenManager:i}=e,r=$a(e,["uid","auth","stsTokenManager"]);this.providerId="firebase",this.proactiveRefresh=new hE(this),this.reloadUserInfo=null,this.reloadListener=null,this.uid=n,this.auth=s,this.stsTokenManager=i,this.accessToken=i.accessToken,this.displayName=r.displayName||null,this.email=r.email||null,this.emailVerified=r.emailVerified||!1,this.phoneNumber=r.phoneNumber||null,this.photoURL=r.photoURL||null,this.isAnonymous=r.isAnonymous||!1,this.tenantId=r.tenantId||null,this.providerData=r.providerData?[...r.providerData]:[],this.metadata=new na(r.createdAt||void 0,r.lastLoginAt||void 0)}async getIdToken(e){const n=await ri(this,this.stsTokenManager.getToken(this.auth,e));return V(n,this.auth,"internal-error"),this.accessToken!==n&&(this.accessToken=n,await this.auth._persistUserIfCurrent(this),this.auth._notifyListenersIfCurrent(this)),n}getIdTokenResult(e){return cE(this,e)}reload(){return dE(this)}_assign(e){this!==e&&(V(this.uid===e.uid,this.auth,"internal-error"),this.displayName=e.displayName,this.photoURL=e.photoURL,this.email=e.email,this.emailVerified=e.emailVerified,this.phoneNumber=e.phoneNumber,this.isAnonymous=e.isAnonymous,this.tenantId=e.tenantId,this.providerData=e.providerData.map(n=>Object.assign({},n)),this.metadata._copy(e.metadata),this.stsTokenManager._assign(e.stsTokenManager))}_clone(e){const n=new Ot(Object.assign(Object.assign({},this),{auth:e,stsTokenManager:this.stsTokenManager._clone()}));return n.metadata._copy(this.metadata),n}_onReload(e){V(!this.reloadListener,this.auth,"internal-error"),this.reloadListener=e,this.reloadUserInfo&&(this._notifyReloadListener(this.reloadUserInfo),this.reloadUserInfo=null)}_notifyReloadListener(e){this.reloadListener?this.reloadListener(e):this.reloadUserInfo=e}_startProactiveRefresh(){this.proactiveRefresh._start()}_stopProactiveRefresh(){this.proactiveRefresh._stop()}async _updateTokensIfNecessary(e,n=!1){let s=!1;e.idToken&&e.idToken!==this.stsTokenManager.accessToken&&(this.stsTokenManager.updateFromServerResponse(e),s=!0),n&&await hr(this),await this.auth._persistUserIfCurrent(this),s&&this.auth._notifyListenersIfCurrent(this)}async delete(){if(it(this.auth.app))return Promise.reject(Mt(this.auth));const e=await this.getIdToken();return await ri(this,lE(this.auth,{idToken:e})),this.stsTokenManager.clearRefreshToken(),this.auth.signOut()}toJSON(){return Object.assign(Object.assign({uid:this.uid,email:this.email||void 0,emailVerified:this.emailVerified,displayName:this.displayName||void 0,isAnonymous:this.isAnonymous,photoURL:this.photoURL||void 0,phoneNumber:this.phoneNumber||void 0,tenantId:this.tenantId||void 0,providerData:this.providerData.map(e=>Object.assign({},e)),stsTokenManager:this.stsTokenManager.toJSON(),_redirectEventId:this._redirectEventId},this.metadata.toJSON()),{apiKey:this.auth.config.apiKey,appName:this.auth.name})}get refreshToken(){return this.stsTokenManager.refreshToken||""}static _fromJSON(e,n){var s,i,r,o,a,l,c,h;const u=(s=n.displayName)!==null&&s!==void 0?s:void 0,d=(i=n.email)!==null&&i!==void 0?i:void 0,_=(r=n.phoneNumber)!==null&&r!==void 0?r:void 0,m=(o=n.photoURL)!==null&&o!==void 0?o:void 0,I=(a=n.tenantId)!==null&&a!==void 0?a:void 0,O=(l=n._redirectEventId)!==null&&l!==void 0?l:void 0,D=(c=n.createdAt)!==null&&c!==void 0?c:void 0,k=(h=n.lastLoginAt)!==null&&h!==void 0?h:void 0,{uid:M,emailVerified:x,isAnonymous:j,providerData:ge,stsTokenManager:X}=n;V(M&&X,e,"internal-error");const q=is.fromJSON(this.name,X);V(typeof M=="string",e,"internal-error"),Kt(u,e.name),Kt(d,e.name),V(typeof x=="boolean",e,"internal-error"),V(typeof j=="boolean",e,"internal-error"),Kt(_,e.name),Kt(m,e.name),Kt(I,e.name),Kt(O,e.name),Kt(D,e.name),Kt(k,e.name);const K=new Ot({uid:M,auth:e,email:d,emailVerified:x,displayName:u,isAnonymous:j,photoURL:m,phoneNumber:_,tenantId:I,stsTokenManager:q,createdAt:D,lastLoginAt:k});return ge&&Array.isArray(ge)&&(K.providerData=ge.map(fe=>Object.assign({},fe))),O&&(K._redirectEventId=O),K}static async _fromIdTokenResponse(e,n,s=!1){const i=new is;i.updateFromServerResponse(n);const r=new Ot({uid:n.localId,auth:e,stsTokenManager:i,isAnonymous:s});return await hr(r),r}static async _fromGetAccountInfoResponse(e,n,s){const i=n.users[0];V(i.localId!==void 0,"internal-error");const r=i.providerUserInfo!==void 0?Vd(i.providerUserInfo):[],o=!(i.email&&i.passwordHash)&&!(r!=null&&r.length),a=new is;a.updateFromIdToken(s);const l=new Ot({uid:i.localId,auth:e,stsTokenManager:a,isAnonymous:o}),c={uid:i.localId,displayName:i.displayName||null,photoURL:i.photoUrl||null,email:i.email||null,emailVerified:i.emailVerified||!1,phoneNumber:i.phoneNumber||null,tenantId:i.tenantId||null,providerData:r,metadata:new na(i.createdAt,i.lastLoginAt),isAnonymous:!(i.email&&i.passwordHash)&&!(r!=null&&r.length)};return Object.assign(l,c),l}}/**
 * @license
 * Copyright 2020 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */const Kc=new Map;function xt(t){Ut(t instanceof Function,"Expected a class definition");let e=Kc.get(t);return e?(Ut(e instanceof t,"Instance stored in cache mismatched with class"),e):(e=new t,Kc.set(t,e),e)}/**
 * @license
 * Copyright 2019 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */class Bd{constructor(){this.type="NONE",this.storage={}}async _isAvailable(){return!0}async _set(e,n){this.storage[e]=n}async _get(e){const n=this.storage[e];return n===void 0?null:n}async _remove(e){delete this.storage[e]}_addListener(e,n){}_removeListener(e,n){}}Bd.type="NONE";const qc=Bd;/**
 * @license
 * Copyright 2019 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */function ji(t,e,n){return`firebase:${t}:${e}:${n}`}class rs{constructor(e,n,s){this.persistence=e,this.auth=n,this.userKey=s;const{config:i,name:r}=this.auth;this.fullUserKey=ji(this.userKey,i.apiKey,r),this.fullPersistenceKey=ji("persistence",i.apiKey,r),this.boundEventHandler=n._onStorageEvent.bind(n),this.persistence._addListener(this.fullUserKey,this.boundEventHandler)}setCurrentUser(e){return this.persistence._set(this.fullUserKey,e.toJSON())}async getCurrentUser(){const e=await this.persistence._get(this.fullUserKey);return e?Ot._fromJSON(this.auth,e):null}removeCurrentUser(){return this.persistence._remove(this.fullUserKey)}savePersistenceForRedirect(){return this.persistence._set(this.fullPersistenceKey,this.persistence.type)}async setPersistence(e){if(this.persistence===e)return;const n=await this.getCurrentUser();if(await this.removeCurrentUser(),this.persistence=e,n)return this.setCurrentUser(n)}delete(){this.persistence._removeListener(this.fullUserKey,this.boundEventHandler)}static async create(e,n,s="authUser"){if(!n.length)return new rs(xt(qc),e,s);const i=(await Promise.all(n.map(async c=>{if(await c._isAvailable())return c}))).filter(c=>c);let r=i[0]||xt(qc);const o=ji(s,e.config.apiKey,e.name);let a=null;for(const c of n)try{const h=await c._get(o);if(h){const u=Ot._fromJSON(e,h);c!==r&&(a=u),r=c;break}}catch{}const l=i.filter(c=>c._shouldAllowMigration);return!r._shouldAllowMigration||!l.length?new rs(r,e,s):(r=l[0],a&&await r._set(o,a.toJSON()),await Promise.all(n.map(async c=>{if(c!==r)try{await c._remove(o)}catch{}})),new rs(r,e,s))}}/**
 * @license
 * Copyright 2020 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */function zc(t){const e=t.toLowerCase();if(e.includes("opera/")||e.includes("opr/")||e.includes("opios/"))return"Opera";if(Kd(e))return"IEMobile";if(e.includes("msie")||e.includes("trident/"))return"IE";if(e.includes("edge/"))return"Edge";if($d(e))return"Firefox";if(e.includes("silk/"))return"Silk";if(zd(e))return"Blackberry";if(Yd(e))return"Webos";if(jd(e))return"Safari";if((e.includes("chrome/")||Gd(e))&&!e.includes("edge/"))return"Chrome";if(qd(e))return"Android";{const n=/([a-zA-Z\d\.]+)\/[a-zA-Z\d\.]*$/,s=t.match(n);if((s==null?void 0:s.length)===2)return s[1]}return"Other"}function $d(t=Fe()){return/firefox\//i.test(t)}function jd(t=Fe()){const e=t.toLowerCase();return e.includes("safari/")&&!e.includes("chrome/")&&!e.includes("crios/")&&!e.includes("android")}function Gd(t=Fe()){return/crios\//i.test(t)}function Kd(t=Fe()){return/iemobile/i.test(t)}function qd(t=Fe()){return/android/i.test(t)}function zd(t=Fe()){return/blackberry/i.test(t)}function Yd(t=Fe()){return/webos/i.test(t)}function za(t=Fe()){return/iphone|ipad|ipod/i.test(t)||/macintosh/i.test(t)&&/mobile/i.test(t)}function gE(t=Fe()){var e;return za(t)&&!!(!((e=window.navigator)===null||e===void 0)&&e.standalone)}function mE(){return Rv()&&document.documentMode===10}function Qd(t=Fe()){return za(t)||qd(t)||Yd(t)||zd(t)||/windows phone/i.test(t)||Kd(t)}/**
 * @license
 * Copyright 2020 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */function Jd(t,e=[]){let n;switch(t){case"Browser":n=zc(Fe());break;case"Worker":n=`${zc(Fe())}-${t}`;break;default:n=t}const s=e.length?e.join(","):"FirebaseCore-web";return`${n}/JsCore/${ys}/${s}`}/**
 * @license
 * Copyright 2022 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */class vE{constructor(e){this.auth=e,this.queue=[]}pushCallback(e,n){const s=r=>new Promise((o,a)=>{try{const l=e(r);o(l)}catch(l){a(l)}});s.onAbort=n,this.queue.push(s);const i=this.queue.length-1;return()=>{this.queue[i]=()=>Promise.resolve()}}async runMiddleware(e){if(this.auth.currentUser===e)return;const n=[];try{for(const s of this.queue)await s(e),s.onAbort&&n.push(s.onAbort)}catch(s){n.reverse();for(const i of n)try{i()}catch{}throw this.auth._errorFactory.create("login-blocked",{originalMessage:s==null?void 0:s.message})}}}/**
 * @license
 * Copyright 2023 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */async function yE(t,e={}){return vn(t,"GET","/v2/passwordPolicy",mn(t,e))}/**
 * @license
 * Copyright 2023 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */const EE=6;class wE{constructor(e){var n,s,i,r;const o=e.customStrengthOptions;this.customStrengthOptions={},this.customStrengthOptions.minPasswordLength=(n=o.minPasswordLength)!==null&&n!==void 0?n:EE,o.maxPasswordLength&&(this.customStrengthOptions.maxPasswordLength=o.maxPasswordLength),o.containsLowercaseCharacter!==void 0&&(this.customStrengthOptions.containsLowercaseLetter=o.containsLowercaseCharacter),o.containsUppercaseCharacter!==void 0&&(this.customStrengthOptions.containsUppercaseLetter=o.containsUppercaseCharacter),o.containsNumericCharacter!==void 0&&(this.customStrengthOptions.containsNumericCharacter=o.containsNumericCharacter),o.containsNonAlphanumericCharacter!==void 0&&(this.customStrengthOptions.containsNonAlphanumericCharacter=o.containsNonAlphanumericCharacter),this.enforcementState=e.enforcementState,this.enforcementState==="ENFORCEMENT_STATE_UNSPECIFIED"&&(this.enforcementState="OFF"),this.allowedNonAlphanumericCharacters=(i=(s=e.allowedNonAlphanumericCharacters)===null||s===void 0?void 0:s.join(""))!==null&&i!==void 0?i:"",this.forceUpgradeOnSignin=(r=e.forceUpgradeOnSignin)!==null&&r!==void 0?r:!1,this.schemaVersion=e.schemaVersion}validatePassword(e){var n,s,i,r,o,a;const l={isValid:!0,passwordPolicy:this};return this.validatePasswordLengthOptions(e,l),this.validatePasswordCharacterOptions(e,l),l.isValid&&(l.isValid=(n=l.meetsMinPasswordLength)!==null&&n!==void 0?n:!0),l.isValid&&(l.isValid=(s=l.meetsMaxPasswordLength)!==null&&s!==void 0?s:!0),l.isValid&&(l.isValid=(i=l.containsLowercaseLetter)!==null&&i!==void 0?i:!0),l.isValid&&(l.isValid=(r=l.containsUppercaseLetter)!==null&&r!==void 0?r:!0),l.isValid&&(l.isValid=(o=l.containsNumericCharacter)!==null&&o!==void 0?o:!0),l.isValid&&(l.isValid=(a=l.containsNonAlphanumericCharacter)!==null&&a!==void 0?a:!0),l}validatePasswordLengthOptions(e,n){const s=this.customStrengthOptions.minPasswordLength,i=this.customStrengthOptions.maxPasswordLength;s&&(n.meetsMinPasswordLength=e.length>=s),i&&(n.meetsMaxPasswordLength=e.length<=i)}validatePasswordCharacterOptions(e,n){this.updatePasswordCharacterOptionsStatuses(n,!1,!1,!1,!1);let s;for(let i=0;i<e.length;i++)s=e.charAt(i),this.updatePasswordCharacterOptionsStatuses(n,s>="a"&&s<="z",s>="A"&&s<="Z",s>="0"&&s<="9",this.allowedNonAlphanumericCharacters.includes(s))}updatePasswordCharacterOptionsStatuses(e,n,s,i,r){this.customStrengthOptions.containsLowercaseLetter&&(e.containsLowercaseLetter||(e.containsLowercaseLetter=n)),this.customStrengthOptions.containsUppercaseLetter&&(e.containsUppercaseLetter||(e.containsUppercaseLetter=s)),this.customStrengthOptions.containsNumericCharacter&&(e.containsNumericCharacter||(e.containsNumericCharacter=i)),this.customStrengthOptions.containsNonAlphanumericCharacter&&(e.containsNonAlphanumericCharacter||(e.containsNonAlphanumericCharacter=r))}}/**
 * @license
 * Copyright 2020 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */class IE{constructor(e,n,s,i){this.app=e,this.heartbeatServiceProvider=n,this.appCheckServiceProvider=s,this.config=i,this.currentUser=null,this.emulatorConfig=null,this.operations=Promise.resolve(),this.authStateSubscription=new Yc(this),this.idTokenSubscription=new Yc(this),this.beforeStateQueue=new vE(this),this.redirectUser=null,this.isProactiveRefreshEnabled=!1,this.EXPECTED_PASSWORD_POLICY_SCHEMA_VERSION=1,this._canInitEmulator=!0,this._isInitialized=!1,this._deleted=!1,this._initializationPromise=null,this._popupRedirectResolver=null,this._errorFactory=Ld,this._agentRecaptchaConfig=null,this._tenantRecaptchaConfigs={},this._projectPasswordPolicy=null,this._tenantPasswordPolicies={},this.lastNotifiedUid=void 0,this.languageCode=null,this.tenantId=null,this.settings={appVerificationDisabledForTesting:!1},this.frameworks=[],this.name=e.name,this.clientVersion=i.sdkClientVersion}_initializeWithPersistence(e,n){return n&&(this._popupRedirectResolver=xt(n)),this._initializationPromise=this.queue(async()=>{var s,i;if(!this._deleted&&(this.persistenceManager=await rs.create(this,e),!this._deleted)){if(!((s=this._popupRedirectResolver)===null||s===void 0)&&s._shouldInitProactively)try{await this._popupRedirectResolver._initialize(this)}catch{}await this.initializeCurrentUser(n),this.lastNotifiedUid=((i=this.currentUser)===null||i===void 0?void 0:i.uid)||null,!this._deleted&&(this._isInitialized=!0)}}),this._initializationPromise}async _onStorageEvent(){if(this._deleted)return;const e=await this.assertedPersistence.getCurrentUser();if(!(!this.currentUser&&!e)){if(this.currentUser&&e&&this.currentUser.uid===e.uid){this._currentUser._assign(e),await this.currentUser.getIdToken();return}await this._updateCurrentUser(e,!0)}}async initializeCurrentUserFromIdToken(e){try{const n=await Hd(this,{idToken:e}),s=await Ot._fromGetAccountInfoResponse(this,n,e);await this.directlySetCurrentUser(s)}catch(n){console.warn("FirebaseServerApp could not login user with provided authIdToken: ",n),await this.directlySetCurrentUser(null)}}async initializeCurrentUser(e){var n;if(it(this.app)){const o=this.app.settings.authIdToken;return o?new Promise(a=>{setTimeout(()=>this.initializeCurrentUserFromIdToken(o).then(a,a))}):this.directlySetCurrentUser(null)}const s=await this.assertedPersistence.getCurrentUser();let i=s,r=!1;if(e&&this.config.authDomain){await this.getOrInitRedirectPersistenceManager();const o=(n=this.redirectUser)===null||n===void 0?void 0:n._redirectEventId,a=i==null?void 0:i._redirectEventId,l=await this.tryRedirectSignIn(e);(!o||o===a)&&(l!=null&&l.user)&&(i=l.user,r=!0)}if(!i)return this.directlySetCurrentUser(null);if(!i._redirectEventId){if(r)try{await this.beforeStateQueue.runMiddleware(i)}catch(o){i=s,this._popupRedirectResolver._overrideRedirectResult(this,()=>Promise.reject(o))}return i?this.reloadAndSetCurrentUserOrClear(i):this.directlySetCurrentUser(null)}return V(this._popupRedirectResolver,this,"argument-error"),await this.getOrInitRedirectPersistenceManager(),this.redirectUser&&this.redirectUser._redirectEventId===i._redirectEventId?this.directlySetCurrentUser(i):this.reloadAndSetCurrentUserOrClear(i)}async tryRedirectSignIn(e){let n=null;try{n=await this._popupRedirectResolver._completeRedirectFn(this,e,!0)}catch{await this._setRedirectUser(null)}return n}async reloadAndSetCurrentUserOrClear(e){try{await hr(e)}catch(n){if((n==null?void 0:n.code)!=="auth/network-request-failed")return this.directlySetCurrentUser(null)}return this.directlySetCurrentUser(e)}useDeviceLanguage(){this.languageCode=tE()}async _delete(){this._deleted=!0}async updateCurrentUser(e){if(it(this.app))return Promise.reject(Mt(this));const n=e?Ue(e):null;return n&&V(n.auth.config.apiKey===this.config.apiKey,this,"invalid-user-token"),this._updateCurrentUser(n&&n._clone(this))}async _updateCurrentUser(e,n=!1){if(!this._deleted)return e&&V(this.tenantId===e.tenantId,this,"tenant-id-mismatch"),n||await this.beforeStateQueue.runMiddleware(e),this.queue(async()=>{await this.directlySetCurrentUser(e),this.notifyAuthListeners()})}async signOut(){return it(this.app)?Promise.reject(Mt(this)):(await this.beforeStateQueue.runMiddleware(null),(this.redirectPersistenceManager||this._popupRedirectResolver)&&await this._setRedirectUser(null),this._updateCurrentUser(null,!0))}setPersistence(e){return it(this.app)?Promise.reject(Mt(this)):this.queue(async()=>{await this.assertedPersistence.setPersistence(xt(e))})}_getRecaptchaConfig(){return this.tenantId==null?this._agentRecaptchaConfig:this._tenantRecaptchaConfigs[this.tenantId]}async validatePassword(e){this._getPasswordPolicyInternal()||await this._updatePasswordPolicy();const n=this._getPasswordPolicyInternal();return n.schemaVersion!==this.EXPECTED_PASSWORD_POLICY_SCHEMA_VERSION?Promise.reject(this._errorFactory.create("unsupported-password-policy-schema-version",{})):n.validatePassword(e)}_getPasswordPolicyInternal(){return this.tenantId===null?this._projectPasswordPolicy:this._tenantPasswordPolicies[this.tenantId]}async _updatePasswordPolicy(){const e=await yE(this),n=new wE(e);this.tenantId===null?this._projectPasswordPolicy=n:this._tenantPasswordPolicies[this.tenantId]=n}_getPersistence(){return this.assertedPersistence.persistence.type}_updateErrorMap(e){this._errorFactory=new Ei("auth","Firebase",e())}onAuthStateChanged(e,n,s){return this.registerStateListener(this.authStateSubscription,e,n,s)}beforeAuthStateChanged(e,n){return this.beforeStateQueue.pushCallback(e,n)}onIdTokenChanged(e,n,s){return this.registerStateListener(this.idTokenSubscription,e,n,s)}authStateReady(){return new Promise((e,n)=>{if(this.currentUser)e();else{const s=this.onAuthStateChanged(()=>{s(),e()},n)}})}async revokeAccessToken(e){if(this.currentUser){const n=await this.currentUser.getIdToken(),s={providerId:"apple.com",tokenType:"ACCESS_TOKEN",token:e,idToken:n};this.tenantId!=null&&(s.tenantId=this.tenantId),await _E(this,s)}}toJSON(){var e;return{apiKey:this.config.apiKey,authDomain:this.config.authDomain,appName:this.name,currentUser:(e=this._currentUser)===null||e===void 0?void 0:e.toJSON()}}async _setRedirectUser(e,n){const s=await this.getOrInitRedirectPersistenceManager(n);return e===null?s.removeCurrentUser():s.setCurrentUser(e)}async getOrInitRedirectPersistenceManager(e){if(!this.redirectPersistenceManager){const n=e&&xt(e)||this._popupRedirectResolver;V(n,this,"argument-error"),this.redirectPersistenceManager=await rs.create(this,[xt(n._redirectPersistence)],"redirectUser"),this.redirectUser=await this.redirectPersistenceManager.getCurrentUser()}return this.redirectPersistenceManager}async _redirectUserForId(e){var n,s;return this._isInitialized&&await this.queue(async()=>{}),((n=this._currentUser)===null||n===void 0?void 0:n._redirectEventId)===e?this._currentUser:((s=this.redirectUser)===null||s===void 0?void 0:s._redirectEventId)===e?this.redirectUser:null}async _persistUserIfCurrent(e){if(e===this.currentUser)return this.queue(async()=>this.directlySetCurrentUser(e))}_notifyListenersIfCurrent(e){e===this.currentUser&&this.notifyAuthListeners()}_key(){return`${this.config.authDomain}:${this.config.apiKey}:${this.name}`}_startProactiveRefresh(){this.isProactiveRefreshEnabled=!0,this.currentUser&&this._currentUser._startProactiveRefresh()}_stopProactiveRefresh(){this.isProactiveRefreshEnabled=!1,this.currentUser&&this._currentUser._stopProactiveRefresh()}get _currentUser(){return this.currentUser}notifyAuthListeners(){var e,n;if(!this._isInitialized)return;this.idTokenSubscription.next(this.currentUser);const s=(n=(e=this.currentUser)===null||e===void 0?void 0:e.uid)!==null&&n!==void 0?n:null;this.lastNotifiedUid!==s&&(this.lastNotifiedUid=s,this.authStateSubscription.next(this.currentUser))}registerStateListener(e,n,s,i){if(this._deleted)return()=>{};const r=typeof n=="function"?n:n.next.bind(n);let o=!1;const a=this._isInitialized?Promise.resolve():this._initializationPromise;if(V(a,this,"internal-error"),a.then(()=>{o||r(this.currentUser)}),typeof n=="function"){const l=e.addObserver(n,s,i);return()=>{o=!0,l()}}else{const l=e.addObserver(n);return()=>{o=!0,l()}}}async directlySetCurrentUser(e){this.currentUser&&this.currentUser!==e&&this._currentUser._stopProactiveRefresh(),e&&this.isProactiveRefreshEnabled&&e._startProactiveRefresh(),this.currentUser=e,e?await this.assertedPersistence.setCurrentUser(e):await this.assertedPersistence.removeCurrentUser()}queue(e){return this.operations=this.operations.then(e,e),this.operations}get assertedPersistence(){return V(this.persistenceManager,this,"internal-error"),this.persistenceManager}_logFramework(e){!e||this.frameworks.includes(e)||(this.frameworks.push(e),this.frameworks.sort(),this.clientVersion=Jd(this.config.clientPlatform,this._getFrameworks()))}_getFrameworks(){return this.frameworks}async _getAdditionalHeaders(){var e;const n={"X-Client-Version":this.clientVersion};this.app.options.appId&&(n["X-Firebase-gmpid"]=this.app.options.appId);const s=await((e=this.heartbeatServiceProvider.getImmediate({optional:!0}))===null||e===void 0?void 0:e.getHeartbeatsHeader());s&&(n["X-Firebase-Client"]=s);const i=await this._getAppCheckToken();return i&&(n["X-Firebase-AppCheck"]=i),n}async _getAppCheckToken(){var e;const n=await((e=this.appCheckServiceProvider.getImmediate({optional:!0}))===null||e===void 0?void 0:e.getToken());return n!=null&&n.error&&Jy(`Error while retrieving App Check token: ${n.error}`),n==null?void 0:n.token}}function yn(t){return Ue(t)}class Yc{constructor(e){this.auth=e,this.observer=null,this.addObserver=Fv(n=>this.observer=n)}get next(){return V(this.observer,this.auth,"internal-error"),this.observer.next.bind(this.observer)}}/**
 * @license
 * Copyright 2020 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */let qr={async loadJS(){throw new Error("Unable to load external scripts")},recaptchaV2Script:"",recaptchaEnterpriseScript:"",gapiScript:""};function bE(t){qr=t}function Xd(t){return qr.loadJS(t)}function CE(){return qr.recaptchaEnterpriseScript}function SE(){return qr.gapiScript}function TE(t){return`__${t}${Math.floor(Math.random()*1e6)}`}const RE="recaptcha-enterprise",PE="NO_RECAPTCHA";class AE{constructor(e){this.type=RE,this.auth=yn(e)}async verify(e="verify",n=!1){async function s(r){if(!n){if(r.tenantId==null&&r._agentRecaptchaConfig!=null)return r._agentRecaptchaConfig.siteKey;if(r.tenantId!=null&&r._tenantRecaptchaConfigs[r.tenantId]!==void 0)return r._tenantRecaptchaConfigs[r.tenantId].siteKey}return new Promise(async(o,a)=>{aE(r,{clientType:"CLIENT_TYPE_WEB",version:"RECAPTCHA_ENTERPRISE"}).then(l=>{if(l.recaptchaKey===void 0)a(new Error("recaptcha Enterprise site key undefined"));else{const c=new oE(l);return r.tenantId==null?r._agentRecaptchaConfig=c:r._tenantRecaptchaConfigs[r.tenantId]=c,o(c.siteKey)}}).catch(l=>{a(l)})})}function i(r,o,a){const l=window.grecaptcha;jc(l)?l.enterprise.ready(()=>{l.enterprise.execute(r,{action:e}).then(c=>{o(c)}).catch(()=>{o(PE)})}):a(Error("No reCAPTCHA enterprise script loaded."))}return new Promise((r,o)=>{s(this.auth).then(a=>{if(!n&&jc(window.grecaptcha))i(a,r,o);else{if(typeof window>"u"){o(new Error("RecaptchaVerifier is only supported in browser"));return}let l=CE();l.length!==0&&(l+=a),Xd(l).then(()=>{i(a,r,o)}).catch(c=>{o(c)})}}).catch(a=>{o(a)})})}}async function Qc(t,e,n,s=!1){const i=new AE(t);let r;try{r=await i.verify(n)}catch{r=await i.verify(n,!0)}const o=Object.assign({},e);return s?Object.assign(o,{captchaResp:r}):Object.assign(o,{captchaResponse:r}),Object.assign(o,{clientType:"CLIENT_TYPE_WEB"}),Object.assign(o,{recaptchaVersion:"RECAPTCHA_ENTERPRISE"}),o}async function sa(t,e,n,s){var i;if(!((i=t._getRecaptchaConfig())===null||i===void 0)&&i.isProviderEnabled("EMAIL_PASSWORD_PROVIDER")){const r=await Qc(t,e,n,n==="getOobCode");return s(t,r)}else return s(t,e).catch(async r=>{if(r.code==="auth/missing-recaptcha-token"){console.log(`${n} is protected by reCAPTCHA Enterprise for this project. Automatically triggering the reCAPTCHA flow and restarting the flow.`);const o=await Qc(t,e,n,n==="getOobCode");return s(t,o)}else return Promise.reject(r)})}/**
 * @license
 * Copyright 2020 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */function NE(t,e){const n=Ba(t,"auth");if(n.isInitialized()){const i=n.getImmediate(),r=n.getOptions();if(lr(r,e??{}))return i;nt(i,"already-initialized")}return n.initialize({options:e})}function kE(t,e){const n=(e==null?void 0:e.persistence)||[],s=(Array.isArray(n)?n:[n]).map(xt);e!=null&&e.errorMap&&t._updateErrorMap(e.errorMap),t._initializeWithPersistence(s,e==null?void 0:e.popupRedirectResolver)}function OE(t,e,n){const s=yn(t);V(s._canInitEmulator,s,"emulator-config-failed"),V(/^https?:\/\//.test(e),s,"invalid-emulator-scheme");const i=!1,r=Zd(e),{host:o,port:a}=xE(e),l=a===null?"":`:${a}`;s.config.emulator={url:`${r}//${o}${l}/`},s.settings.appVerificationDisabledForTesting=!0,s.emulatorConfig=Object.freeze({host:o,port:a,protocol:r.replace(":",""),options:Object.freeze({disableWarnings:i})}),DE()}function Zd(t){const e=t.indexOf(":");return e<0?"":t.substr(0,e+1)}function xE(t){const e=Zd(t),n=/(\/\/)?([^?#/]+)/.exec(t.substr(e.length));if(!n)return{host:"",port:null};const s=n[2].split("@").pop()||"",i=/^(\[[^\]]+\])(:|$)/.exec(s);if(i){const r=i[1];return{host:r,port:Jc(s.substr(r.length+1))}}else{const[r,o]=s.split(":");return{host:r,port:Jc(o)}}}function Jc(t){if(!t)return null;const e=Number(t);return isNaN(e)?null:e}function DE(){function t(){const e=document.createElement("p"),n=e.style;e.innerText="Running in emulator mode. Do not use with production credentials.",n.position="fixed",n.width="100%",n.backgroundColor="#ffffff",n.border=".1em solid #000000",n.color="#b50000",n.bottom="0px",n.left="0px",n.margin="0px",n.zIndex="10000",n.textAlign="center",e.classList.add("firebase-emulator-warning"),document.body.appendChild(e)}typeof console<"u"&&typeof console.info=="function"&&console.info("WARNING: You are using the Auth Emulator, which is intended for local testing only.  Do not use with production credentials."),typeof window<"u"&&typeof document<"u"&&(document.readyState==="loading"?window.addEventListener("DOMContentLoaded",t):t())}/**
 * @license
 * Copyright 2020 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */class Ya{constructor(e,n){this.providerId=e,this.signInMethod=n}toJSON(){return kt("not implemented")}_getIdTokenResponse(e){return kt("not implemented")}_linkToIdToken(e,n){return kt("not implemented")}_getReauthenticationResolver(e){return kt("not implemented")}}async function ME(t,e){return vn(t,"POST","/v1/accounts:signUp",e)}/**
 * @license
 * Copyright 2020 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */async function LE(t,e){return Ii(t,"POST","/v1/accounts:signInWithPassword",mn(t,e))}/**
 * @license
 * Copyright 2020 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */async function FE(t,e){return Ii(t,"POST","/v1/accounts:signInWithEmailLink",mn(t,e))}async function UE(t,e){return Ii(t,"POST","/v1/accounts:signInWithEmailLink",mn(t,e))}/**
 * @license
 * Copyright 2020 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */class oi extends Ya{constructor(e,n,s,i=null){super("password",s),this._email=e,this._password=n,this._tenantId=i}static _fromEmailAndPassword(e,n){return new oi(e,n,"password")}static _fromEmailAndCode(e,n,s=null){return new oi(e,n,"emailLink",s)}toJSON(){return{email:this._email,password:this._password,signInMethod:this.signInMethod,tenantId:this._tenantId}}static fromJSON(e){const n=typeof e=="string"?JSON.parse(e):e;if(n!=null&&n.email&&(n!=null&&n.password)){if(n.signInMethod==="password")return this._fromEmailAndPassword(n.email,n.password);if(n.signInMethod==="emailLink")return this._fromEmailAndCode(n.email,n.password,n.tenantId)}return null}async _getIdTokenResponse(e){switch(this.signInMethod){case"password":const n={returnSecureToken:!0,email:this._email,password:this._password,clientType:"CLIENT_TYPE_WEB"};return sa(e,n,"signInWithPassword",LE);case"emailLink":return FE(e,{email:this._email,oobCode:this._password});default:nt(e,"internal-error")}}async _linkToIdToken(e,n){switch(this.signInMethod){case"password":const s={idToken:n,returnSecureToken:!0,email:this._email,password:this._password,clientType:"CLIENT_TYPE_WEB"};return sa(e,s,"signUpPassword",ME);case"emailLink":return UE(e,{idToken:n,email:this._email,oobCode:this._password});default:nt(e,"internal-error")}}_getReauthenticationResolver(e){return this._getIdTokenResponse(e)}}/**
 * @license
 * Copyright 2020 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */async function os(t,e){return Ii(t,"POST","/v1/accounts:signInWithIdp",mn(t,e))}/**
 * @license
 * Copyright 2020 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */const WE="http://localhost";class Fn extends Ya{constructor(){super(...arguments),this.pendingToken=null}static _fromParams(e){const n=new Fn(e.providerId,e.signInMethod);return e.idToken||e.accessToken?(e.idToken&&(n.idToken=e.idToken),e.accessToken&&(n.accessToken=e.accessToken),e.nonce&&!e.pendingToken&&(n.nonce=e.nonce),e.pendingToken&&(n.pendingToken=e.pendingToken)):e.oauthToken&&e.oauthTokenSecret?(n.accessToken=e.oauthToken,n.secret=e.oauthTokenSecret):nt("argument-error"),n}toJSON(){return{idToken:this.idToken,accessToken:this.accessToken,secret:this.secret,nonce:this.nonce,pendingToken:this.pendingToken,providerId:this.providerId,signInMethod:this.signInMethod}}static fromJSON(e){const n=typeof e=="string"?JSON.parse(e):e,{providerId:s,signInMethod:i}=n,r=$a(n,["providerId","signInMethod"]);if(!s||!i)return null;const o=new Fn(s,i);return o.idToken=r.idToken||void 0,o.accessToken=r.accessToken||void 0,o.secret=r.secret,o.nonce=r.nonce,o.pendingToken=r.pendingToken||null,o}_getIdTokenResponse(e){const n=this.buildRequest();return os(e,n)}_linkToIdToken(e,n){const s=this.buildRequest();return s.idToken=n,os(e,s)}_getReauthenticationResolver(e){const n=this.buildRequest();return n.autoCreate=!1,os(e,n)}buildRequest(){const e={requestUri:WE,returnSecureToken:!0};if(this.pendingToken)e.pendingToken=this.pendingToken;else{const n={};this.idToken&&(n.id_token=this.idToken),this.accessToken&&(n.access_token=this.accessToken),this.secret&&(n.oauth_token_secret=this.secret),n.providerId=this.providerId,this.nonce&&!this.pendingToken&&(n.nonce=this.nonce),e.postBody=vs(n)}return e}}/**
 * @license
 * Copyright 2020 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */function HE(t){switch(t){case"recoverEmail":return"RECOVER_EMAIL";case"resetPassword":return"PASSWORD_RESET";case"signIn":return"EMAIL_SIGNIN";case"verifyEmail":return"VERIFY_EMAIL";case"verifyAndChangeEmail":return"VERIFY_AND_CHANGE_EMAIL";case"revertSecondFactorAddition":return"REVERT_SECOND_FACTOR_ADDITION";default:return null}}function VE(t){const e=xs(Ds(t)).link,n=e?xs(Ds(e)).deep_link_id:null,s=xs(Ds(t)).deep_link_id;return(s?xs(Ds(s)).link:null)||s||n||e||t}class Qa{constructor(e){var n,s,i,r,o,a;const l=xs(Ds(e)),c=(n=l.apiKey)!==null&&n!==void 0?n:null,h=(s=l.oobCode)!==null&&s!==void 0?s:null,u=HE((i=l.mode)!==null&&i!==void 0?i:null);V(c&&h&&u,"argument-error"),this.apiKey=c,this.operation=u,this.code=h,this.continueUrl=(r=l.continueUrl)!==null&&r!==void 0?r:null,this.languageCode=(o=l.languageCode)!==null&&o!==void 0?o:null,this.tenantId=(a=l.tenantId)!==null&&a!==void 0?a:null}static parseLink(e){const n=VE(e);try{return new Qa(n)}catch{return null}}}/**
 * @license
 * Copyright 2020 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */class Es{constructor(){this.providerId=Es.PROVIDER_ID}static credential(e,n){return oi._fromEmailAndPassword(e,n)}static credentialWithLink(e,n){const s=Qa.parseLink(n);return V(s,"argument-error"),oi._fromEmailAndCode(e,s.code,s.tenantId)}}Es.PROVIDER_ID="password";Es.EMAIL_PASSWORD_SIGN_IN_METHOD="password";Es.EMAIL_LINK_SIGN_IN_METHOD="emailLink";/**
 * @license
 * Copyright 2020 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */class Ja{constructor(e){this.providerId=e,this.defaultLanguageCode=null,this.customParameters={}}setDefaultLanguage(e){this.defaultLanguageCode=e}setCustomParameters(e){return this.customParameters=e,this}getCustomParameters(){return this.customParameters}}/**
 * @license
 * Copyright 2019 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */class bi extends Ja{constructor(){super(...arguments),this.scopes=[]}addScope(e){return this.scopes.includes(e)||this.scopes.push(e),this}getScopes(){return[...this.scopes]}}/**
 * @license
 * Copyright 2020 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */class Xt extends bi{constructor(){super("facebook.com")}static credential(e){return Fn._fromParams({providerId:Xt.PROVIDER_ID,signInMethod:Xt.FACEBOOK_SIGN_IN_METHOD,accessToken:e})}static credentialFromResult(e){return Xt.credentialFromTaggedObject(e)}static credentialFromError(e){return Xt.credentialFromTaggedObject(e.customData||{})}static credentialFromTaggedObject({_tokenResponse:e}){if(!e||!("oauthAccessToken"in e)||!e.oauthAccessToken)return null;try{return Xt.credential(e.oauthAccessToken)}catch{return null}}}Xt.FACEBOOK_SIGN_IN_METHOD="facebook.com";Xt.PROVIDER_ID="facebook.com";/**
 * @license
 * Copyright 2020 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */class Zt extends bi{constructor(){super("google.com"),this.addScope("profile")}static credential(e,n){return Fn._fromParams({providerId:Zt.PROVIDER_ID,signInMethod:Zt.GOOGLE_SIGN_IN_METHOD,idToken:e,accessToken:n})}static credentialFromResult(e){return Zt.credentialFromTaggedObject(e)}static credentialFromError(e){return Zt.credentialFromTaggedObject(e.customData||{})}static credentialFromTaggedObject({_tokenResponse:e}){if(!e)return null;const{oauthIdToken:n,oauthAccessToken:s}=e;if(!n&&!s)return null;try{return Zt.credential(n,s)}catch{return null}}}Zt.GOOGLE_SIGN_IN_METHOD="google.com";Zt.PROVIDER_ID="google.com";/**
 * @license
 * Copyright 2020 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */class en extends bi{constructor(){super("github.com")}static credential(e){return Fn._fromParams({providerId:en.PROVIDER_ID,signInMethod:en.GITHUB_SIGN_IN_METHOD,accessToken:e})}static credentialFromResult(e){return en.credentialFromTaggedObject(e)}static credentialFromError(e){return en.credentialFromTaggedObject(e.customData||{})}static credentialFromTaggedObject({_tokenResponse:e}){if(!e||!("oauthAccessToken"in e)||!e.oauthAccessToken)return null;try{return en.credential(e.oauthAccessToken)}catch{return null}}}en.GITHUB_SIGN_IN_METHOD="github.com";en.PROVIDER_ID="github.com";/**
 * @license
 * Copyright 2020 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */class tn extends bi{constructor(){super("twitter.com")}static credential(e,n){return Fn._fromParams({providerId:tn.PROVIDER_ID,signInMethod:tn.TWITTER_SIGN_IN_METHOD,oauthToken:e,oauthTokenSecret:n})}static credentialFromResult(e){return tn.credentialFromTaggedObject(e)}static credentialFromError(e){return tn.credentialFromTaggedObject(e.customData||{})}static credentialFromTaggedObject({_tokenResponse:e}){if(!e)return null;const{oauthAccessToken:n,oauthTokenSecret:s}=e;if(!n||!s)return null;try{return tn.credential(n,s)}catch{return null}}}tn.TWITTER_SIGN_IN_METHOD="twitter.com";tn.PROVIDER_ID="twitter.com";/**
 * @license
 * Copyright 2020 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */async function BE(t,e){return Ii(t,"POST","/v1/accounts:signUp",mn(t,e))}/**
 * @license
 * Copyright 2020 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */class Un{constructor(e){this.user=e.user,this.providerId=e.providerId,this._tokenResponse=e._tokenResponse,this.operationType=e.operationType}static async _fromIdTokenResponse(e,n,s,i=!1){const r=await Ot._fromIdTokenResponse(e,s,i),o=Xc(s);return new Un({user:r,providerId:o,_tokenResponse:s,operationType:n})}static async _forOperation(e,n,s){await e._updateTokensIfNecessary(s,!0);const i=Xc(s);return new Un({user:e,providerId:i,_tokenResponse:s,operationType:n})}}function Xc(t){return t.providerId?t.providerId:"phoneNumber"in t?"phone":null}/**
 * @license
 * Copyright 2020 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */class dr extends gn{constructor(e,n,s,i){var r;super(n.code,n.message),this.operationType=s,this.user=i,Object.setPrototypeOf(this,dr.prototype),this.customData={appName:e.name,tenantId:(r=e.tenantId)!==null&&r!==void 0?r:void 0,_serverResponse:n.customData._serverResponse,operationType:s}}static _fromErrorAndOperation(e,n,s,i){return new dr(e,n,s,i)}}function ef(t,e,n,s){return(e==="reauthenticate"?n._getReauthenticationResolver(t):n._getIdTokenResponse(t)).catch(r=>{throw r.code==="auth/multi-factor-auth-required"?dr._fromErrorAndOperation(t,r,e,s):r})}async function $E(t,e,n=!1){const s=await ri(t,e._linkToIdToken(t.auth,await t.getIdToken()),n);return Un._forOperation(t,"link",s)}/**
 * @license
 * Copyright 2019 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */async function jE(t,e,n=!1){const{auth:s}=t;if(it(s.app))return Promise.reject(Mt(s));const i="reauthenticate";try{const r=await ri(t,ef(s,i,e,t),n);V(r.idToken,s,"internal-error");const o=qa(r.idToken);V(o,s,"internal-error");const{sub:a}=o;return V(t.uid===a,s,"user-mismatch"),Un._forOperation(t,i,r)}catch(r){throw(r==null?void 0:r.code)==="auth/user-not-found"&&nt(s,"user-mismatch"),r}}/**
 * @license
 * Copyright 2020 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */async function tf(t,e,n=!1){if(it(t.app))return Promise.reject(Mt(t));const s="signIn",i=await ef(t,s,e),r=await Un._fromIdTokenResponse(t,s,i);return n||await t._updateCurrentUser(r.user),r}async function GE(t,e){return tf(yn(t),e)}/**
 * @license
 * Copyright 2020 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */async function nf(t){const e=yn(t);e._getPasswordPolicyInternal()&&await e._updatePasswordPolicy()}async function jS(t,e,n){if(it(t.app))return Promise.reject(Mt(t));const s=yn(t),o=await sa(s,{returnSecureToken:!0,email:e,password:n,clientType:"CLIENT_TYPE_WEB"},"signUpPassword",BE).catch(l=>{throw l.code==="auth/password-does-not-meet-requirements"&&nf(t),l}),a=await Un._fromIdTokenResponse(s,"signIn",o);return await s._updateCurrentUser(a.user),a}function GS(t,e,n){return it(t.app)?Promise.reject(Mt(t)):GE(Ue(t),Es.credential(e,n)).catch(async s=>{throw s.code==="auth/password-does-not-meet-requirements"&&nf(t),s})}function KE(t,e,n,s){return Ue(t).onIdTokenChanged(e,n,s)}function qE(t,e,n){return Ue(t).beforeAuthStateChanged(e,n)}function zE(t,e,n,s){return Ue(t).onAuthStateChanged(e,n,s)}const fr="__sak";/**
 * @license
 * Copyright 2019 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */class sf{constructor(e,n){this.storageRetriever=e,this.type=n}_isAvailable(){try{return this.storage?(this.storage.setItem(fr,"1"),this.storage.removeItem(fr),Promise.resolve(!0)):Promise.resolve(!1)}catch{return Promise.resolve(!1)}}_set(e,n){return this.storage.setItem(e,JSON.stringify(n)),Promise.resolve()}_get(e){const n=this.storage.getItem(e);return Promise.resolve(n?JSON.parse(n):null)}_remove(e){return this.storage.removeItem(e),Promise.resolve()}get storage(){return this.storageRetriever()}}/**
 * @license
 * Copyright 2020 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */const YE=1e3,QE=10;class rf extends sf{constructor(){super(()=>window.localStorage,"LOCAL"),this.boundEventHandler=(e,n)=>this.onStorageEvent(e,n),this.listeners={},this.localCache={},this.pollTimer=null,this.fallbackToPolling=Qd(),this._shouldAllowMigration=!0}forAllChangedKeys(e){for(const n of Object.keys(this.listeners)){const s=this.storage.getItem(n),i=this.localCache[n];s!==i&&e(n,i,s)}}onStorageEvent(e,n=!1){if(!e.key){this.forAllChangedKeys((o,a,l)=>{this.notifyListeners(o,l)});return}const s=e.key;n?this.detachListener():this.stopPolling();const i=()=>{const o=this.storage.getItem(s);!n&&this.localCache[s]===o||this.notifyListeners(s,o)},r=this.storage.getItem(s);mE()&&r!==e.newValue&&e.newValue!==e.oldValue?setTimeout(i,QE):i()}notifyListeners(e,n){this.localCache[e]=n;const s=this.listeners[e];if(s)for(const i of Array.from(s))i(n&&JSON.parse(n))}startPolling(){this.stopPolling(),this.pollTimer=setInterval(()=>{this.forAllChangedKeys((e,n,s)=>{this.onStorageEvent(new StorageEvent("storage",{key:e,oldValue:n,newValue:s}),!0)})},YE)}stopPolling(){this.pollTimer&&(clearInterval(this.pollTimer),this.pollTimer=null)}attachListener(){window.addEventListener("storage",this.boundEventHandler)}detachListener(){window.removeEventListener("storage",this.boundEventHandler)}_addListener(e,n){Object.keys(this.listeners).length===0&&(this.fallbackToPolling?this.startPolling():this.attachListener()),this.listeners[e]||(this.listeners[e]=new Set,this.localCache[e]=this.storage.getItem(e)),this.listeners[e].add(n)}_removeListener(e,n){this.listeners[e]&&(this.listeners[e].delete(n),this.listeners[e].size===0&&delete this.listeners[e]),Object.keys(this.listeners).length===0&&(this.detachListener(),this.stopPolling())}async _set(e,n){await super._set(e,n),this.localCache[e]=JSON.stringify(n)}async _get(e){const n=await super._get(e);return this.localCache[e]=JSON.stringify(n),n}async _remove(e){await super._remove(e),delete this.localCache[e]}}rf.type="LOCAL";const JE=rf;/**
 * @license
 * Copyright 2020 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */class of extends sf{constructor(){super(()=>window.sessionStorage,"SESSION")}_addListener(e,n){}_removeListener(e,n){}}of.type="SESSION";const af=of;/**
 * @license
 * Copyright 2019 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */function XE(t){return Promise.all(t.map(async e=>{try{return{fulfilled:!0,value:await e}}catch(n){return{fulfilled:!1,reason:n}}}))}/**
 * @license
 * Copyright 2019 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */class zr{constructor(e){this.eventTarget=e,this.handlersMap={},this.boundEventHandler=this.handleEvent.bind(this)}static _getInstance(e){const n=this.receivers.find(i=>i.isListeningto(e));if(n)return n;const s=new zr(e);return this.receivers.push(s),s}isListeningto(e){return this.eventTarget===e}async handleEvent(e){const n=e,{eventId:s,eventType:i,data:r}=n.data,o=this.handlersMap[i];if(!(o!=null&&o.size))return;n.ports[0].postMessage({status:"ack",eventId:s,eventType:i});const a=Array.from(o).map(async c=>c(n.origin,r)),l=await XE(a);n.ports[0].postMessage({status:"done",eventId:s,eventType:i,response:l})}_subscribe(e,n){Object.keys(this.handlersMap).length===0&&this.eventTarget.addEventListener("message",this.boundEventHandler),this.handlersMap[e]||(this.handlersMap[e]=new Set),this.handlersMap[e].add(n)}_unsubscribe(e,n){this.handlersMap[e]&&n&&this.handlersMap[e].delete(n),(!n||this.handlersMap[e].size===0)&&delete this.handlersMap[e],Object.keys(this.handlersMap).length===0&&this.eventTarget.removeEventListener("message",this.boundEventHandler)}}zr.receivers=[];/**
 * @license
 * Copyright 2020 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */function Xa(t="",e=10){let n="";for(let s=0;s<e;s++)n+=Math.floor(Math.random()*10);return t+n}/**
 * @license
 * Copyright 2019 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */class ZE{constructor(e){this.target=e,this.handlers=new Set}removeMessageHandler(e){e.messageChannel&&(e.messageChannel.port1.removeEventListener("message",e.onMessage),e.messageChannel.port1.close()),this.handlers.delete(e)}async _send(e,n,s=50){const i=typeof MessageChannel<"u"?new MessageChannel:null;if(!i)throw new Error("connection_unavailable");let r,o;return new Promise((a,l)=>{const c=Xa("",20);i.port1.start();const h=setTimeout(()=>{l(new Error("unsupported_event"))},s);o={messageChannel:i,onMessage(u){const d=u;if(d.data.eventId===c)switch(d.data.status){case"ack":clearTimeout(h),r=setTimeout(()=>{l(new Error("timeout"))},3e3);break;case"done":clearTimeout(r),a(d.data.response);break;default:clearTimeout(h),clearTimeout(r),l(new Error("invalid_response"));break}}},this.handlers.add(o),i.port1.addEventListener("message",o.onMessage),this.target.postMessage({eventType:e,eventId:c,data:n},[i.port2])}).finally(()=>{o&&this.removeMessageHandler(o)})}}/**
 * @license
 * Copyright 2020 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */function yt(){return window}function ew(t){yt().location.href=t}/**
 * @license
 * Copyright 2020 Google LLC.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */function lf(){return typeof yt().WorkerGlobalScope<"u"&&typeof yt().importScripts=="function"}async function tw(){if(!(navigator!=null&&navigator.serviceWorker))return null;try{return(await navigator.serviceWorker.ready).active}catch{return null}}function nw(){var t;return((t=navigator==null?void 0:navigator.serviceWorker)===null||t===void 0?void 0:t.controller)||null}function sw(){return lf()?self:null}/**
 * @license
 * Copyright 2019 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */const cf="firebaseLocalStorageDb",iw=1,pr="firebaseLocalStorage",uf="fbase_key";class Ci{constructor(e){this.request=e}toPromise(){return new Promise((e,n)=>{this.request.addEventListener("success",()=>{e(this.request.result)}),this.request.addEventListener("error",()=>{n(this.request.error)})})}}function Yr(t,e){return t.transaction([pr],e?"readwrite":"readonly").objectStore(pr)}function rw(){const t=indexedDB.deleteDatabase(cf);return new Ci(t).toPromise()}function ia(){const t=indexedDB.open(cf,iw);return new Promise((e,n)=>{t.addEventListener("error",()=>{n(t.error)}),t.addEventListener("upgradeneeded",()=>{const s=t.result;try{s.createObjectStore(pr,{keyPath:uf})}catch(i){n(i)}}),t.addEventListener("success",async()=>{const s=t.result;s.objectStoreNames.contains(pr)?e(s):(s.close(),await rw(),e(await ia()))})})}async function Zc(t,e,n){const s=Yr(t,!0).put({[uf]:e,value:n});return new Ci(s).toPromise()}async function ow(t,e){const n=Yr(t,!1).get(e),s=await new Ci(n).toPromise();return s===void 0?null:s.value}function eu(t,e){const n=Yr(t,!0).delete(e);return new Ci(n).toPromise()}const aw=800,lw=3;class hf{constructor(){this.type="LOCAL",this._shouldAllowMigration=!0,this.listeners={},this.localCache={},this.pollTimer=null,this.pendingWrites=0,this.receiver=null,this.sender=null,this.serviceWorkerReceiverAvailable=!1,this.activeServiceWorker=null,this._workerInitializationPromise=this.initializeServiceWorkerMessaging().then(()=>{},()=>{})}async _openDb(){return this.db?this.db:(this.db=await ia(),this.db)}async _withRetries(e){let n=0;for(;;)try{const s=await this._openDb();return await e(s)}catch(s){if(n++>lw)throw s;this.db&&(this.db.close(),this.db=void 0)}}async initializeServiceWorkerMessaging(){return lf()?this.initializeReceiver():this.initializeSender()}async initializeReceiver(){this.receiver=zr._getInstance(sw()),this.receiver._subscribe("keyChanged",async(e,n)=>({keyProcessed:(await this._poll()).includes(n.key)})),this.receiver._subscribe("ping",async(e,n)=>["keyChanged"])}async initializeSender(){var e,n;if(this.activeServiceWorker=await tw(),!this.activeServiceWorker)return;this.sender=new ZE(this.activeServiceWorker);const s=await this.sender._send("ping",{},800);s&&!((e=s[0])===null||e===void 0)&&e.fulfilled&&!((n=s[0])===null||n===void 0)&&n.value.includes("keyChanged")&&(this.serviceWorkerReceiverAvailable=!0)}async notifyServiceWorker(e){if(!(!this.sender||!this.activeServiceWorker||nw()!==this.activeServiceWorker))try{await this.sender._send("keyChanged",{key:e},this.serviceWorkerReceiverAvailable?800:50)}catch{}}async _isAvailable(){try{if(!indexedDB)return!1;const e=await ia();return await Zc(e,fr,"1"),await eu(e,fr),!0}catch{}return!1}async _withPendingWrite(e){this.pendingWrites++;try{await e()}finally{this.pendingWrites--}}async _set(e,n){return this._withPendingWrite(async()=>(await this._withRetries(s=>Zc(s,e,n)),this.localCache[e]=n,this.notifyServiceWorker(e)))}async _get(e){const n=await this._withRetries(s=>ow(s,e));return this.localCache[e]=n,n}async _remove(e){return this._withPendingWrite(async()=>(await this._withRetries(n=>eu(n,e)),delete this.localCache[e],this.notifyServiceWorker(e)))}async _poll(){const e=await this._withRetries(i=>{const r=Yr(i,!1).getAll();return new Ci(r).toPromise()});if(!e)return[];if(this.pendingWrites!==0)return[];const n=[],s=new Set;if(e.length!==0)for(const{fbase_key:i,value:r}of e)s.add(i),JSON.stringify(this.localCache[i])!==JSON.stringify(r)&&(this.notifyListeners(i,r),n.push(i));for(const i of Object.keys(this.localCache))this.localCache[i]&&!s.has(i)&&(this.notifyListeners(i,null),n.push(i));return n}notifyListeners(e,n){this.localCache[e]=n;const s=this.listeners[e];if(s)for(const i of Array.from(s))i(n)}startPolling(){this.stopPolling(),this.pollTimer=setInterval(async()=>this._poll(),aw)}stopPolling(){this.pollTimer&&(clearInterval(this.pollTimer),this.pollTimer=null)}_addListener(e,n){Object.keys(this.listeners).length===0&&this.startPolling(),this.listeners[e]||(this.listeners[e]=new Set,this._get(e)),this.listeners[e].add(n)}_removeListener(e,n){this.listeners[e]&&(this.listeners[e].delete(n),this.listeners[e].size===0&&delete this.listeners[e]),Object.keys(this.listeners).length===0&&this.stopPolling()}}hf.type="LOCAL";const cw=hf;new wi(3e4,6e4);/**
 * @license
 * Copyright 2021 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */function df(t,e){return e?xt(e):(V(t._popupRedirectResolver,t,"argument-error"),t._popupRedirectResolver)}/**
 * @license
 * Copyright 2019 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */class Za extends Ya{constructor(e){super("custom","custom"),this.params=e}_getIdTokenResponse(e){return os(e,this._buildIdpRequest())}_linkToIdToken(e,n){return os(e,this._buildIdpRequest(n))}_getReauthenticationResolver(e){return os(e,this._buildIdpRequest())}_buildIdpRequest(e){const n={requestUri:this.params.requestUri,sessionId:this.params.sessionId,postBody:this.params.postBody,tenantId:this.params.tenantId,pendingToken:this.params.pendingToken,returnSecureToken:!0,returnIdpCredential:!0};return e&&(n.idToken=e),n}}function uw(t){return tf(t.auth,new Za(t),t.bypassAuthState)}function hw(t){const{auth:e,user:n}=t;return V(n,e,"internal-error"),jE(n,new Za(t),t.bypassAuthState)}async function dw(t){const{auth:e,user:n}=t;return V(n,e,"internal-error"),$E(n,new Za(t),t.bypassAuthState)}/**
 * @license
 * Copyright 2020 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */class ff{constructor(e,n,s,i,r=!1){this.auth=e,this.resolver=s,this.user=i,this.bypassAuthState=r,this.pendingPromise=null,this.eventManager=null,this.filter=Array.isArray(n)?n:[n]}execute(){return new Promise(async(e,n)=>{this.pendingPromise={resolve:e,reject:n};try{this.eventManager=await this.resolver._initialize(this.auth),await this.onExecution(),this.eventManager.registerConsumer(this)}catch(s){this.reject(s)}})}async onAuthEvent(e){const{urlResponse:n,sessionId:s,postBody:i,tenantId:r,error:o,type:a}=e;if(o){this.reject(o);return}const l={auth:this.auth,requestUri:n,sessionId:s,tenantId:r||void 0,postBody:i||void 0,user:this.user,bypassAuthState:this.bypassAuthState};try{this.resolve(await this.getIdpTask(a)(l))}catch(c){this.reject(c)}}onError(e){this.reject(e)}getIdpTask(e){switch(e){case"signInViaPopup":case"signInViaRedirect":return uw;case"linkViaPopup":case"linkViaRedirect":return dw;case"reauthViaPopup":case"reauthViaRedirect":return hw;default:nt(this.auth,"internal-error")}}resolve(e){Ut(this.pendingPromise,"Pending promise was never set"),this.pendingPromise.resolve(e),this.unregisterAndCleanUp()}reject(e){Ut(this.pendingPromise,"Pending promise was never set"),this.pendingPromise.reject(e),this.unregisterAndCleanUp()}unregisterAndCleanUp(){this.eventManager&&this.eventManager.unregisterConsumer(this),this.pendingPromise=null,this.cleanUp()}}/**
 * @license
 * Copyright 2020 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */const fw=new wi(2e3,1e4);async function KS(t,e,n){if(it(t.app))return Promise.reject(lt(t,"operation-not-supported-in-this-environment"));const s=yn(t);Xy(t,e,Ja);const i=df(s,n);return new An(s,"signInViaPopup",e,i).executeNotNull()}class An extends ff{constructor(e,n,s,i,r){super(e,n,i,r),this.provider=s,this.authWindow=null,this.pollId=null,An.currentPopupAction&&An.currentPopupAction.cancel(),An.currentPopupAction=this}async executeNotNull(){const e=await this.execute();return V(e,this.auth,"internal-error"),e}async onExecution(){Ut(this.filter.length===1,"Popup operations only handle one event");const e=Xa();this.authWindow=await this.resolver._openPopup(this.auth,this.provider,this.filter[0],e),this.authWindow.associatedEvent=e,this.resolver._originValidation(this.auth).catch(n=>{this.reject(n)}),this.resolver._isIframeWebStorageSupported(this.auth,n=>{n||this.reject(lt(this.auth,"web-storage-unsupported"))}),this.pollUserCancellation()}get eventId(){var e;return((e=this.authWindow)===null||e===void 0?void 0:e.associatedEvent)||null}cancel(){this.reject(lt(this.auth,"cancelled-popup-request"))}cleanUp(){this.authWindow&&this.authWindow.close(),this.pollId&&window.clearTimeout(this.pollId),this.authWindow=null,this.pollId=null,An.currentPopupAction=null}pollUserCancellation(){const e=()=>{var n,s;if(!((s=(n=this.authWindow)===null||n===void 0?void 0:n.window)===null||s===void 0)&&s.closed){this.pollId=window.setTimeout(()=>{this.pollId=null,this.reject(lt(this.auth,"popup-closed-by-user"))},8e3);return}this.pollId=window.setTimeout(e,fw.get())};e()}}An.currentPopupAction=null;/**
 * @license
 * Copyright 2020 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */const pw="pendingRedirect",Gi=new Map;class _w extends ff{constructor(e,n,s=!1){super(e,["signInViaRedirect","linkViaRedirect","reauthViaRedirect","unknown"],n,void 0,s),this.eventId=null}async execute(){let e=Gi.get(this.auth._key());if(!e){try{const s=await gw(this.resolver,this.auth)?await super.execute():null;e=()=>Promise.resolve(s)}catch(n){e=()=>Promise.reject(n)}Gi.set(this.auth._key(),e)}return this.bypassAuthState||Gi.set(this.auth._key(),()=>Promise.resolve(null)),e()}async onAuthEvent(e){if(e.type==="signInViaRedirect")return super.onAuthEvent(e);if(e.type==="unknown"){this.resolve(null);return}if(e.eventId){const n=await this.auth._redirectUserForId(e.eventId);if(n)return this.user=n,super.onAuthEvent(e);this.resolve(null)}}async onExecution(){}cleanUp(){}}async function gw(t,e){const n=yw(e),s=vw(t);if(!await s._isAvailable())return!1;const i=await s._get(n)==="true";return await s._remove(n),i}function mw(t,e){Gi.set(t._key(),e)}function vw(t){return xt(t._redirectPersistence)}function yw(t){return ji(pw,t.config.apiKey,t.name)}async function Ew(t,e,n=!1){if(it(t.app))return Promise.reject(Mt(t));const s=yn(t),i=df(s,e),o=await new _w(s,i,n).execute();return o&&!n&&(delete o.user._redirectEventId,await s._persistUserIfCurrent(o.user),await s._setRedirectUser(null,e)),o}/**
 * @license
 * Copyright 2020 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */const ww=10*60*1e3;class Iw{constructor(e){this.auth=e,this.cachedEventUids=new Set,this.consumers=new Set,this.queuedRedirectEvent=null,this.hasHandledPotentialRedirect=!1,this.lastProcessedEventTime=Date.now()}registerConsumer(e){this.consumers.add(e),this.queuedRedirectEvent&&this.isEventForConsumer(this.queuedRedirectEvent,e)&&(this.sendToConsumer(this.queuedRedirectEvent,e),this.saveEventToCache(this.queuedRedirectEvent),this.queuedRedirectEvent=null)}unregisterConsumer(e){this.consumers.delete(e)}onEvent(e){if(this.hasEventBeenHandled(e))return!1;let n=!1;return this.consumers.forEach(s=>{this.isEventForConsumer(e,s)&&(n=!0,this.sendToConsumer(e,s),this.saveEventToCache(e))}),this.hasHandledPotentialRedirect||!bw(e)||(this.hasHandledPotentialRedirect=!0,n||(this.queuedRedirectEvent=e,n=!0)),n}sendToConsumer(e,n){var s;if(e.error&&!pf(e)){const i=((s=e.error.code)===null||s===void 0?void 0:s.split("auth/")[1])||"internal-error";n.onError(lt(this.auth,i))}else n.onAuthEvent(e)}isEventForConsumer(e,n){const s=n.eventId===null||!!e.eventId&&e.eventId===n.eventId;return n.filter.includes(e.type)&&s}hasEventBeenHandled(e){return Date.now()-this.lastProcessedEventTime>=ww&&this.cachedEventUids.clear(),this.cachedEventUids.has(tu(e))}saveEventToCache(e){this.cachedEventUids.add(tu(e)),this.lastProcessedEventTime=Date.now()}}function tu(t){return[t.type,t.eventId,t.sessionId,t.tenantId].filter(e=>e).join("-")}function pf({type:t,error:e}){return t==="unknown"&&(e==null?void 0:e.code)==="auth/no-auth-event"}function bw(t){switch(t.type){case"signInViaRedirect":case"linkViaRedirect":case"reauthViaRedirect":return!0;case"unknown":return pf(t);default:return!1}}/**
 * @license
 * Copyright 2020 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */async function Cw(t,e={}){return vn(t,"GET","/v1/projects",e)}/**
 * @license
 * Copyright 2020 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */const Sw=/^\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}$/,Tw=/^https?/;async function Rw(t){if(t.config.emulator)return;const{authorizedDomains:e}=await Cw(t);for(const n of e)try{if(Pw(n))return}catch{}nt(t,"unauthorized-domain")}function Pw(t){const e=ta(),{protocol:n,hostname:s}=new URL(e);if(t.startsWith("chrome-extension://")){const o=new URL(t);return o.hostname===""&&s===""?n==="chrome-extension:"&&t.replace("chrome-extension://","")===e.replace("chrome-extension://",""):n==="chrome-extension:"&&o.hostname===s}if(!Tw.test(n))return!1;if(Sw.test(t))return s===t;const i=t.replace(/\./g,"\\.");return new RegExp("^(.+\\."+i+"|"+i+")$","i").test(s)}/**
 * @license
 * Copyright 2020 Google LLC.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */const Aw=new wi(3e4,6e4);function nu(){const t=yt().___jsl;if(t!=null&&t.H){for(const e of Object.keys(t.H))if(t.H[e].r=t.H[e].r||[],t.H[e].L=t.H[e].L||[],t.H[e].r=[...t.H[e].L],t.CP)for(let n=0;n<t.CP.length;n++)t.CP[n]=null}}function Nw(t){return new Promise((e,n)=>{var s,i,r;function o(){nu(),gapi.load("gapi.iframes",{callback:()=>{e(gapi.iframes.getContext())},ontimeout:()=>{nu(),n(lt(t,"network-request-failed"))},timeout:Aw.get()})}if(!((i=(s=yt().gapi)===null||s===void 0?void 0:s.iframes)===null||i===void 0)&&i.Iframe)e(gapi.iframes.getContext());else if(!((r=yt().gapi)===null||r===void 0)&&r.load)o();else{const a=TE("iframefcb");return yt()[a]=()=>{gapi.load?o():n(lt(t,"network-request-failed"))},Xd(`${SE()}?onload=${a}`).catch(l=>n(l))}}).catch(e=>{throw Ki=null,e})}let Ki=null;function kw(t){return Ki=Ki||Nw(t),Ki}/**
 * @license
 * Copyright 2020 Google LLC.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */const Ow=new wi(5e3,15e3),xw="__/auth/iframe",Dw="emulator/auth/iframe",Mw={style:{position:"absolute",top:"-100px",width:"1px",height:"1px"},"aria-hidden":"true",tabindex:"-1"},Lw=new Map([["identitytoolkit.googleapis.com","p"],["staging-identitytoolkit.sandbox.googleapis.com","s"],["test-identitytoolkit.sandbox.googleapis.com","t"]]);function Fw(t){const e=t.config;V(e.authDomain,t,"auth-domain-config-required");const n=e.emulator?Ka(e,Dw):`https://${t.config.authDomain}/${xw}`,s={apiKey:e.apiKey,appName:t.name,v:ys},i=Lw.get(t.config.apiHost);i&&(s.eid=i);const r=t._getFrameworks();return r.length&&(s.fw=r.join(",")),`${n}?${vs(s).slice(1)}`}async function Uw(t){const e=await kw(t),n=yt().gapi;return V(n,t,"internal-error"),e.open({where:document.body,url:Fw(t),messageHandlersFilter:n.iframes.CROSS_ORIGIN_IFRAMES_FILTER,attributes:Mw,dontclear:!0},s=>new Promise(async(i,r)=>{await s.restyle({setHideOnLeave:!1});const o=lt(t,"network-request-failed"),a=yt().setTimeout(()=>{r(o)},Ow.get());function l(){yt().clearTimeout(a),i(s)}s.ping(l).then(l,()=>{r(o)})}))}/**
 * @license
 * Copyright 2020 Google LLC.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */const Ww={location:"yes",resizable:"yes",statusbar:"yes",toolbar:"no"},Hw=500,Vw=600,Bw="_blank",$w="http://localhost";class su{constructor(e){this.window=e,this.associatedEvent=null}close(){if(this.window)try{this.window.close()}catch{}}}function jw(t,e,n,s=Hw,i=Vw){const r=Math.max((window.screen.availHeight-i)/2,0).toString(),o=Math.max((window.screen.availWidth-s)/2,0).toString();let a="";const l=Object.assign(Object.assign({},Ww),{width:s.toString(),height:i.toString(),top:r,left:o}),c=Fe().toLowerCase();n&&(a=Gd(c)?Bw:n),$d(c)&&(e=e||$w,l.scrollbars="yes");const h=Object.entries(l).reduce((d,[_,m])=>`${d}${_}=${m},`,"");if(gE(c)&&a!=="_self")return Gw(e||"",a),new su(null);const u=window.open(e||"",a,h);V(u,t,"popup-blocked");try{u.focus()}catch{}return new su(u)}function Gw(t,e){const n=document.createElement("a");n.href=t,n.target=e;const s=document.createEvent("MouseEvent");s.initMouseEvent("click",!0,!0,window,1,0,0,0,0,!1,!1,!1,!1,1,null),n.dispatchEvent(s)}/**
 * @license
 * Copyright 2021 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */const Kw="__/auth/handler",qw="emulator/auth/handler",zw=encodeURIComponent("fac");async function iu(t,e,n,s,i,r){V(t.config.authDomain,t,"auth-domain-config-required"),V(t.config.apiKey,t,"invalid-api-key");const o={apiKey:t.config.apiKey,appName:t.name,authType:n,redirectUrl:s,v:ys,eventId:i};if(e instanceof Ja){e.setDefaultLanguage(t.languageCode),o.providerId=e.providerId||"",Yo(e.getCustomParameters())||(o.customParameters=JSON.stringify(e.getCustomParameters()));for(const[h,u]of Object.entries({}))o[h]=u}if(e instanceof bi){const h=e.getScopes().filter(u=>u!=="");h.length>0&&(o.scopes=h.join(","))}t.tenantId&&(o.tid=t.tenantId);const a=o;for(const h of Object.keys(a))a[h]===void 0&&delete a[h];const l=await t._getAppCheckToken(),c=l?`#${zw}=${encodeURIComponent(l)}`:"";return`${Yw(t)}?${vs(a).slice(1)}${c}`}function Yw({config:t}){return t.emulator?Ka(t,qw):`https://${t.authDomain}/${Kw}`}/**
 * @license
 * Copyright 2020 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */const To="webStorageSupport";class Qw{constructor(){this.eventManagers={},this.iframes={},this.originValidationPromises={},this._redirectPersistence=af,this._completeRedirectFn=Ew,this._overrideRedirectResult=mw}async _openPopup(e,n,s,i){var r;Ut((r=this.eventManagers[e._key()])===null||r===void 0?void 0:r.manager,"_initialize() not called before _openPopup()");const o=await iu(e,n,s,ta(),i);return jw(e,o,Xa())}async _openRedirect(e,n,s,i){await this._originValidation(e);const r=await iu(e,n,s,ta(),i);return ew(r),new Promise(()=>{})}_initialize(e){const n=e._key();if(this.eventManagers[n]){const{manager:i,promise:r}=this.eventManagers[n];return i?Promise.resolve(i):(Ut(r,"If manager is not set, promise should be"),r)}const s=this.initAndGetManager(e);return this.eventManagers[n]={promise:s},s.catch(()=>{delete this.eventManagers[n]}),s}async initAndGetManager(e){const n=await Uw(e),s=new Iw(e);return n.register("authEvent",i=>(V(i==null?void 0:i.authEvent,e,"invalid-auth-event"),{status:s.onEvent(i.authEvent)?"ACK":"ERROR"}),gapi.iframes.CROSS_ORIGIN_IFRAMES_FILTER),this.eventManagers[e._key()]={manager:s},this.iframes[e._key()]=n,s}_isIframeWebStorageSupported(e,n){this.iframes[e._key()].send(To,{type:To},i=>{var r;const o=(r=i==null?void 0:i[0])===null||r===void 0?void 0:r[To];o!==void 0&&n(!!o),nt(e,"internal-error")},gapi.iframes.CROSS_ORIGIN_IFRAMES_FILTER)}_originValidation(e){const n=e._key();return this.originValidationPromises[n]||(this.originValidationPromises[n]=Rw(e)),this.originValidationPromises[n]}get _shouldInitProactively(){return Qd()||jd()||za()}}const Jw=Qw;var ru="@firebase/auth",ou="1.7.9";/**
 * @license
 * Copyright 2020 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */class Xw{constructor(e){this.auth=e,this.internalListeners=new Map}getUid(){var e;return this.assertAuthConfigured(),((e=this.auth.currentUser)===null||e===void 0?void 0:e.uid)||null}async getToken(e){return this.assertAuthConfigured(),await this.auth._initializationPromise,this.auth.currentUser?{accessToken:await this.auth.currentUser.getIdToken(e)}:null}addAuthTokenListener(e){if(this.assertAuthConfigured(),this.internalListeners.has(e))return;const n=this.auth.onIdTokenChanged(s=>{e((s==null?void 0:s.stsTokenManager.accessToken)||null)});this.internalListeners.set(e,n),this.updateProactiveRefresh()}removeAuthTokenListener(e){this.assertAuthConfigured();const n=this.internalListeners.get(e);n&&(this.internalListeners.delete(e),n(),this.updateProactiveRefresh())}assertAuthConfigured(){V(this.auth._initializationPromise,"dependent-sdk-initialized-before-auth")}updateProactiveRefresh(){this.internalListeners.size>0?this.auth._startProactiveRefresh():this.auth._stopProactiveRefresh()}}/**
 * @license
 * Copyright 2020 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */function Zw(t){switch(t){case"Node":return"node";case"ReactNative":return"rn";case"Worker":return"webworker";case"Cordova":return"cordova";case"WebExtension":return"web-extension";default:return}}function eI(t){fs(new Ln("auth",(e,{options:n})=>{const s=e.getProvider("app").getImmediate(),i=e.getProvider("heartbeat"),r=e.getProvider("app-check-internal"),{apiKey:o,authDomain:a}=s.options;V(o&&!o.includes(":"),"invalid-api-key",{appName:s.name});const l={apiKey:o,authDomain:a,clientPlatform:t,apiHost:"identitytoolkit.googleapis.com",tokenApiHost:"securetoken.googleapis.com",apiScheme:"https",sdkClientVersion:Jd(t)},c=new IE(s,i,r,l);return kE(c,n),c},"PUBLIC").setInstantiationMode("EXPLICIT").setInstanceCreatedCallback((e,n,s)=>{e.getProvider("auth-internal").initialize()})),fs(new Ln("auth-internal",e=>{const n=yn(e.getProvider("auth").getImmediate());return(s=>new Xw(s))(n)},"PRIVATE").setInstantiationMode("EXPLICIT")),ln(ru,ou,Zw(t)),ln(ru,ou,"esm2017")}/**
 * @license
 * Copyright 2021 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */const tI=5*60,nI=Td("authIdTokenMaxAge")||tI;let au=null;const sI=t=>async e=>{const n=e&&await e.getIdTokenResult(),s=n&&(new Date().getTime()-Date.parse(n.issuedAtTime))/1e3;if(s&&s>nI)return;const i=n==null?void 0:n.token;au!==i&&(au=i,await fetch(t,{method:i?"POST":"DELETE",headers:i?{Authorization:`Bearer ${i}`}:{}}))};function iI(t=Od()){const e=Ba(t,"auth");if(e.isInitialized())return e.getImmediate();const n=NE(t,{popupRedirectResolver:Jw,persistence:[cw,JE,af]}),s=Td("authTokenSyncURL");if(s&&typeof isSecureContext=="boolean"&&isSecureContext){const r=new URL(s,location.origin);if(location.origin===r.origin){const o=sI(r.toString());qE(n,o,()=>o(n.currentUser)),KE(n,a=>o(a))}}const i=Cd("auth");return i&&OE(n,`http://${i}`),n}function rI(){var t,e;return(e=(t=document.getElementsByTagName("head"))===null||t===void 0?void 0:t[0])!==null&&e!==void 0?e:document}bE({loadJS(t){return new Promise((e,n)=>{const s=document.createElement("script");s.setAttribute("src",t),s.onload=e,s.onerror=i=>{const r=lt("internal-error");r.customData=i,n(r)},s.type="text/javascript",s.charset="UTF-8",rI().appendChild(s)})},gapiScript:"https://apis.google.com/js/api.js",recaptchaV2Script:"https://www.google.com/recaptcha/api.js",recaptchaEnterpriseScript:"https://www.google.com/recaptcha/enterprise.js?render="});eI("Browser");var lu={};const cu="@firebase/database",uu="1.0.8";/**
 * @license
 * Copyright 2019 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */let _f="";function oI(t){_f=t}/**
 * @license
 * Copyright 2017 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */class aI{constructor(e){this.domStorage_=e,this.prefix_="firebase:"}set(e,n){n==null?this.domStorage_.removeItem(this.prefixedName_(e)):this.domStorage_.setItem(this.prefixedName_(e),we(n))}get(e){const n=this.domStorage_.getItem(this.prefixedName_(e));return n==null?null:si(n)}remove(e){this.domStorage_.removeItem(this.prefixedName_(e))}prefixedName_(e){return this.prefix_+e}toString(){return this.domStorage_.toString()}}/**
 * @license
 * Copyright 2017 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */class lI{constructor(){this.cache_={},this.isInMemoryStorage=!0}set(e,n){n==null?delete this.cache_[e]:this.cache_[e]=n}get(e){return Vt(this.cache_,e)?this.cache_[e]:null}remove(e){delete this.cache_[e]}}/**
 * @license
 * Copyright 2017 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */const gf=function(t){try{if(typeof window<"u"&&typeof window[t]<"u"){const e=window[t];return e.setItem("firebase:sentinel","cache"),e.removeItem("firebase:sentinel"),new aI(e)}}catch{}return new lI},Nn=gf("localStorage"),cI=gf("sessionStorage");/**
 * @license
 * Copyright 2017 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */const as=new Ha("@firebase/database"),uI=function(){let t=1;return function(){return t++}}(),mf=function(t){const e=Hv(t),n=new Lv;n.update(e);const s=n.digest();return La.encodeByteArray(s)},Si=function(...t){let e="";for(let n=0;n<t.length;n++){const s=t[n];Array.isArray(s)||s&&typeof s=="object"&&typeof s.length=="number"?e+=Si.apply(null,s):typeof s=="object"?e+=we(s):e+=s,e+=" "}return e};let Gs=null,hu=!0;const hI=function(t,e){C(!0,"Can't turn on custom loggers persistently."),as.logLevel=re.VERBOSE,Gs=as.log.bind(as)},Ne=function(...t){if(hu===!0&&(hu=!1,Gs===null&&cI.get("logging_enabled")===!0&&hI()),Gs){const e=Si.apply(null,t);Gs(e)}},Ti=function(t){return function(...e){Ne(t,...e)}},ra=function(...t){const e="FIREBASE INTERNAL ERROR: "+Si(...t);as.error(e)},Wt=function(...t){const e=`FIREBASE FATAL ERROR: ${Si(...t)}`;throw as.error(e),new Error(e)},je=function(...t){const e="FIREBASE WARNING: "+Si(...t);as.warn(e)},dI=function(){typeof window<"u"&&window.location&&window.location.protocol&&window.location.protocol.indexOf("https:")!==-1&&je("Insecure Firebase access from a secure page. Please use https in calls to new Firebase().")},vf=function(t){return typeof t=="number"&&(t!==t||t===Number.POSITIVE_INFINITY||t===Number.NEGATIVE_INFINITY)},fI=function(t){if(document.readyState==="complete")t();else{let e=!1;const n=function(){if(!document.body){setTimeout(n,Math.floor(10));return}e||(e=!0,t())};document.addEventListener?(document.addEventListener("DOMContentLoaded",n,!1),window.addEventListener("load",n,!1)):document.attachEvent&&(document.attachEvent("onreadystatechange",()=>{document.readyState==="complete"&&n()}),window.attachEvent("onload",n))}},ps="[MIN_NAME]",Wn="[MAX_NAME]",ws=function(t,e){if(t===e)return 0;if(t===ps||e===Wn)return-1;if(e===ps||t===Wn)return 1;{const n=du(t),s=du(e);return n!==null?s!==null?n-s===0?t.length-e.length:n-s:-1:s!==null?1:t<e?-1:1}},pI=function(t,e){return t===e?0:t<e?-1:1},Ps=function(t,e){if(e&&t in e)return e[t];throw new Error("Missing required key ("+t+") in object: "+we(e))},el=function(t){if(typeof t!="object"||t===null)return we(t);const e=[];for(const s in t)e.push(s);e.sort();let n="{";for(let s=0;s<e.length;s++)s!==0&&(n+=","),n+=we(e[s]),n+=":",n+=el(t[e[s]]);return n+="}",n},yf=function(t,e){const n=t.length;if(n<=e)return[t];const s=[];for(let i=0;i<n;i+=e)i+e>n?s.push(t.substring(i,n)):s.push(t.substring(i,i+e));return s};function Ge(t,e){for(const n in t)t.hasOwnProperty(n)&&e(n,t[n])}const Ef=function(t){C(!vf(t),"Invalid JSON number");const e=11,n=52,s=(1<<e-1)-1;let i,r,o,a,l;t===0?(r=0,o=0,i=1/t===-1/0?1:0):(i=t<0,t=Math.abs(t),t>=Math.pow(2,1-s)?(a=Math.min(Math.floor(Math.log(t)/Math.LN2),s),r=a+s,o=Math.round(t*Math.pow(2,n-a)-Math.pow(2,n))):(r=0,o=Math.round(t/Math.pow(2,1-s-n))));const c=[];for(l=n;l;l-=1)c.push(o%2?1:0),o=Math.floor(o/2);for(l=e;l;l-=1)c.push(r%2?1:0),r=Math.floor(r/2);c.push(i?1:0),c.reverse();const h=c.join("");let u="";for(l=0;l<64;l+=8){let d=parseInt(h.substr(l,8),2).toString(16);d.length===1&&(d="0"+d),u=u+d}return u.toLowerCase()},_I=function(){return!!(typeof window=="object"&&window.chrome&&window.chrome.extension&&!/^chrome/.test(window.location.href))},gI=function(){return typeof Windows=="object"&&typeof Windows.UI=="object"};function mI(t,e){let n="Unknown Error";t==="too_big"?n="The data requested exceeds the maximum size that can be accessed with a single request.":t==="permission_denied"?n="Client doesn't have permission to access the desired data.":t==="unavailable"&&(n="The service is unavailable");const s=new Error(t+" at "+e._path.toString()+": "+n);return s.code=t.toUpperCase(),s}const vI=new RegExp("^-?(0*)\\d{1,10}$"),yI=-2147483648,EI=2147483647,du=function(t){if(vI.test(t)){const e=Number(t);if(e>=yI&&e<=EI)return e}return null},Is=function(t){try{t()}catch(e){setTimeout(()=>{const n=e.stack||"";throw je("Exception was thrown by user callback.",n),e},Math.floor(0))}},wI=function(){return(typeof window=="object"&&window.navigator&&window.navigator.userAgent||"").search(/googlebot|google webmaster tools|bingbot|yahoo! slurp|baiduspider|yandexbot|duckduckbot/i)>=0},Ks=function(t,e){const n=setTimeout(t,e);return typeof n=="number"&&typeof Deno<"u"&&Deno.unrefTimer?Deno.unrefTimer(n):typeof n=="object"&&n.unref&&n.unref(),n};/**
 * @license
 * Copyright 2021 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */class II{constructor(e,n){this.appName_=e,this.appCheckProvider=n,this.appCheck=n==null?void 0:n.getImmediate({optional:!0}),this.appCheck||n==null||n.get().then(s=>this.appCheck=s)}getToken(e){return this.appCheck?this.appCheck.getToken(e):new Promise((n,s)=>{setTimeout(()=>{this.appCheck?this.getToken(e).then(n,s):n(null)},0)})}addTokenChangeListener(e){var n;(n=this.appCheckProvider)===null||n===void 0||n.get().then(s=>s.addTokenListener(e))}notifyForInvalidToken(){je(`Provided AppCheck credentials for the app named "${this.appName_}" are invalid. This usually indicates your app was not initialized correctly.`)}}/**
 * @license
 * Copyright 2017 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */class bI{constructor(e,n,s){this.appName_=e,this.firebaseOptions_=n,this.authProvider_=s,this.auth_=null,this.auth_=s.getImmediate({optional:!0}),this.auth_||s.onInit(i=>this.auth_=i)}getToken(e){return this.auth_?this.auth_.getToken(e).catch(n=>n&&n.code==="auth/token-not-initialized"?(Ne("Got auth/token-not-initialized error.  Treating as null token."),null):Promise.reject(n)):new Promise((n,s)=>{setTimeout(()=>{this.auth_?this.getToken(e).then(n,s):n(null)},0)})}addTokenChangeListener(e){this.auth_?this.auth_.addAuthTokenListener(e):this.authProvider_.get().then(n=>n.addAuthTokenListener(e))}removeTokenChangeListener(e){this.authProvider_.get().then(n=>n.removeAuthTokenListener(e))}notifyForInvalidToken(){let e='Provided authentication credentials for the app named "'+this.appName_+'" are invalid. This usually indicates your app was not initialized correctly. ';"credential"in this.firebaseOptions_?e+='Make sure the "credential" property provided to initializeApp() is authorized to access the specified "databaseURL" and is from the correct project.':"serviceAccount"in this.firebaseOptions_?e+='Make sure the "serviceAccount" property provided to initializeApp() is authorized to access the specified "databaseURL" and is from the correct project.':e+='Make sure the "apiKey" and "databaseURL" properties provided to initializeApp() match the values provided for your app at https://console.firebase.google.com/.',je(e)}}class qi{constructor(e){this.accessToken=e}getToken(e){return Promise.resolve({accessToken:this.accessToken})}addTokenChangeListener(e){e(this.accessToken)}removeTokenChangeListener(e){}notifyForInvalidToken(){}}qi.OWNER="owner";/**
 * @license
 * Copyright 2017 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */const tl="5",wf="v",If="s",bf="r",Cf="f",Sf=/(console\.firebase|firebase-console-\w+\.corp|firebase\.corp)\.google\.com/,Tf="ls",Rf="p",oa="ac",Pf="websocket",Af="long_polling";/**
 * @license
 * Copyright 2017 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */class Nf{constructor(e,n,s,i,r=!1,o="",a=!1,l=!1){this.secure=n,this.namespace=s,this.webSocketOnly=i,this.nodeAdmin=r,this.persistenceKey=o,this.includeNamespaceInQueryParams=a,this.isUsingEmulator=l,this._host=e.toLowerCase(),this._domain=this._host.substr(this._host.indexOf(".")+1),this.internalHost=Nn.get("host:"+e)||this._host}isCacheableHost(){return this.internalHost.substr(0,2)==="s-"}isCustomHost(){return this._domain!=="firebaseio.com"&&this._domain!=="firebaseio-demo.com"}get host(){return this._host}set host(e){e!==this.internalHost&&(this.internalHost=e,this.isCacheableHost()&&Nn.set("host:"+this._host,this.internalHost))}toString(){let e=this.toURLString();return this.persistenceKey&&(e+="<"+this.persistenceKey+">"),e}toURLString(){const e=this.secure?"https://":"http://",n=this.includeNamespaceInQueryParams?`?ns=${this.namespace}`:"";return`${e}${this.host}/${n}`}}function CI(t){return t.host!==t.internalHost||t.isCustomHost()||t.includeNamespaceInQueryParams}function kf(t,e,n){C(typeof e=="string","typeof type must == string"),C(typeof n=="object","typeof params must == object");let s;if(e===Pf)s=(t.secure?"wss://":"ws://")+t.internalHost+"/.ws?";else if(e===Af)s=(t.secure?"https://":"http://")+t.internalHost+"/.lp?";else throw new Error("Unknown connection type: "+e);CI(t)&&(n.ns=t.namespace);const i=[];return Ge(n,(r,o)=>{i.push(r+"="+o)}),s+i.join("&")}/**
 * @license
 * Copyright 2017 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */class SI{constructor(){this.counters_={}}incrementCounter(e,n=1){Vt(this.counters_,e)||(this.counters_[e]=0),this.counters_[e]+=n}get(){return mv(this.counters_)}}/**
 * @license
 * Copyright 2017 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */const Ro={},Po={};function nl(t){const e=t.toString();return Ro[e]||(Ro[e]=new SI),Ro[e]}function TI(t,e){const n=t.toString();return Po[n]||(Po[n]=e()),Po[n]}/**
 * @license
 * Copyright 2017 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */class RI{constructor(e){this.onMessage_=e,this.pendingResponses=[],this.currentResponseNum=0,this.closeAfterResponse=-1,this.onClose=null}closeAfter(e,n){this.closeAfterResponse=e,this.onClose=n,this.closeAfterResponse<this.currentResponseNum&&(this.onClose(),this.onClose=null)}handleResponse(e,n){for(this.pendingResponses[e]=n;this.pendingResponses[this.currentResponseNum];){const s=this.pendingResponses[this.currentResponseNum];delete this.pendingResponses[this.currentResponseNum];for(let i=0;i<s.length;++i)s[i]&&Is(()=>{this.onMessage_(s[i])});if(this.currentResponseNum===this.closeAfterResponse){this.onClose&&(this.onClose(),this.onClose=null);break}this.currentResponseNum++}}}/**
 * @license
 * Copyright 2017 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */const fu="start",PI="close",AI="pLPCommand",NI="pRTLPCB",Of="id",xf="pw",Df="ser",kI="cb",OI="seg",xI="ts",DI="d",MI="dframe",Mf=1870,Lf=30,LI=Mf-Lf,FI=25e3,UI=3e4;class Xn{constructor(e,n,s,i,r,o,a){this.connId=e,this.repoInfo=n,this.applicationId=s,this.appCheckToken=i,this.authToken=r,this.transportSessionId=o,this.lastSessionId=a,this.bytesSent=0,this.bytesReceived=0,this.everConnected_=!1,this.log_=Ti(e),this.stats_=nl(n),this.urlFn=l=>(this.appCheckToken&&(l[oa]=this.appCheckToken),kf(n,Af,l))}open(e,n){this.curSegmentNum=0,this.onDisconnect_=n,this.myPacketOrderer=new RI(e),this.isClosed_=!1,this.connectTimeoutTimer_=setTimeout(()=>{this.log_("Timed out trying to connect."),this.onClosed_(),this.connectTimeoutTimer_=null},Math.floor(UI)),fI(()=>{if(this.isClosed_)return;this.scriptTagHolder=new sl((...r)=>{const[o,a,l,c,h]=r;if(this.incrementIncomingBytes_(r),!!this.scriptTagHolder)if(this.connectTimeoutTimer_&&(clearTimeout(this.connectTimeoutTimer_),this.connectTimeoutTimer_=null),this.everConnected_=!0,o===fu)this.id=a,this.password=l;else if(o===PI)a?(this.scriptTagHolder.sendNewPolls=!1,this.myPacketOrderer.closeAfter(a,()=>{this.onClosed_()})):this.onClosed_();else throw new Error("Unrecognized command received: "+o)},(...r)=>{const[o,a]=r;this.incrementIncomingBytes_(r),this.myPacketOrderer.handleResponse(o,a)},()=>{this.onClosed_()},this.urlFn);const s={};s[fu]="t",s[Df]=Math.floor(Math.random()*1e8),this.scriptTagHolder.uniqueCallbackIdentifier&&(s[kI]=this.scriptTagHolder.uniqueCallbackIdentifier),s[wf]=tl,this.transportSessionId&&(s[If]=this.transportSessionId),this.lastSessionId&&(s[Tf]=this.lastSessionId),this.applicationId&&(s[Rf]=this.applicationId),this.appCheckToken&&(s[oa]=this.appCheckToken),typeof location<"u"&&location.hostname&&Sf.test(location.hostname)&&(s[bf]=Cf);const i=this.urlFn(s);this.log_("Connecting via long-poll to "+i),this.scriptTagHolder.addTag(i,()=>{})})}start(){this.scriptTagHolder.startLongPoll(this.id,this.password),this.addDisconnectPingFrame(this.id,this.password)}static forceAllow(){Xn.forceAllow_=!0}static forceDisallow(){Xn.forceDisallow_=!0}static isAvailable(){return Xn.forceAllow_?!0:!Xn.forceDisallow_&&typeof document<"u"&&document.createElement!=null&&!_I()&&!gI()}markConnectionHealthy(){}shutdown_(){this.isClosed_=!0,this.scriptTagHolder&&(this.scriptTagHolder.close(),this.scriptTagHolder=null),this.myDisconnFrame&&(document.body.removeChild(this.myDisconnFrame),this.myDisconnFrame=null),this.connectTimeoutTimer_&&(clearTimeout(this.connectTimeoutTimer_),this.connectTimeoutTimer_=null)}onClosed_(){this.isClosed_||(this.log_("Longpoll is closing itself"),this.shutdown_(),this.onDisconnect_&&(this.onDisconnect_(this.everConnected_),this.onDisconnect_=null))}close(){this.isClosed_||(this.log_("Longpoll is being closed."),this.shutdown_())}send(e){const n=we(e);this.bytesSent+=n.length,this.stats_.incrementCounter("bytes_sent",n.length);const s=Id(n),i=yf(s,LI);for(let r=0;r<i.length;r++)this.scriptTagHolder.enqueueSegment(this.curSegmentNum,i.length,i[r]),this.curSegmentNum++}addDisconnectPingFrame(e,n){this.myDisconnFrame=document.createElement("iframe");const s={};s[MI]="t",s[Of]=e,s[xf]=n,this.myDisconnFrame.src=this.urlFn(s),this.myDisconnFrame.style.display="none",document.body.appendChild(this.myDisconnFrame)}incrementIncomingBytes_(e){const n=we(e).length;this.bytesReceived+=n,this.stats_.incrementCounter("bytes_received",n)}}class sl{constructor(e,n,s,i){this.onDisconnect=s,this.urlFn=i,this.outstandingRequests=new Set,this.pendingSegs=[],this.currentSerial=Math.floor(Math.random()*1e8),this.sendNewPolls=!0;{this.uniqueCallbackIdentifier=uI(),window[AI+this.uniqueCallbackIdentifier]=e,window[NI+this.uniqueCallbackIdentifier]=n,this.myIFrame=sl.createIFrame_();let r="";this.myIFrame.src&&this.myIFrame.src.substr(0,11)==="javascript:"&&(r='<script>document.domain="'+document.domain+'";<\/script>');const o="<html><body>"+r+"</body></html>";try{this.myIFrame.doc.open(),this.myIFrame.doc.write(o),this.myIFrame.doc.close()}catch(a){Ne("frame writing exception"),a.stack&&Ne(a.stack),Ne(a)}}}static createIFrame_(){const e=document.createElement("iframe");if(e.style.display="none",document.body){document.body.appendChild(e);try{e.contentWindow.document||Ne("No IE domain setting required")}catch{const s=document.domain;e.src="javascript:void((function(){document.open();document.domain='"+s+"';document.close();})())"}}else throw"Document body has not initialized. Wait to initialize Firebase until after the document is ready.";return e.contentDocument?e.doc=e.contentDocument:e.contentWindow?e.doc=e.contentWindow.document:e.document&&(e.doc=e.document),e}close(){this.alive=!1,this.myIFrame&&(this.myIFrame.doc.body.textContent="",setTimeout(()=>{this.myIFrame!==null&&(document.body.removeChild(this.myIFrame),this.myIFrame=null)},Math.floor(0)));const e=this.onDisconnect;e&&(this.onDisconnect=null,e())}startLongPoll(e,n){for(this.myID=e,this.myPW=n,this.alive=!0;this.newRequest_(););}newRequest_(){if(this.alive&&this.sendNewPolls&&this.outstandingRequests.size<(this.pendingSegs.length>0?2:1)){this.currentSerial++;const e={};e[Of]=this.myID,e[xf]=this.myPW,e[Df]=this.currentSerial;let n=this.urlFn(e),s="",i=0;for(;this.pendingSegs.length>0&&this.pendingSegs[0].d.length+Lf+s.length<=Mf;){const o=this.pendingSegs.shift();s=s+"&"+OI+i+"="+o.seg+"&"+xI+i+"="+o.ts+"&"+DI+i+"="+o.d,i++}return n=n+s,this.addLongPollTag_(n,this.currentSerial),!0}else return!1}enqueueSegment(e,n,s){this.pendingSegs.push({seg:e,ts:n,d:s}),this.alive&&this.newRequest_()}addLongPollTag_(e,n){this.outstandingRequests.add(n);const s=()=>{this.outstandingRequests.delete(n),this.newRequest_()},i=setTimeout(s,Math.floor(FI)),r=()=>{clearTimeout(i),s()};this.addTag(e,r)}addTag(e,n){setTimeout(()=>{try{if(!this.sendNewPolls)return;const s=this.myIFrame.doc.createElement("script");s.type="text/javascript",s.async=!0,s.src=e,s.onload=s.onreadystatechange=function(){const i=s.readyState;(!i||i==="loaded"||i==="complete")&&(s.onload=s.onreadystatechange=null,s.parentNode&&s.parentNode.removeChild(s),n())},s.onerror=()=>{Ne("Long-poll script failed to load: "+e),this.sendNewPolls=!1,this.close()},this.myIFrame.doc.body.appendChild(s)}catch{}},Math.floor(1))}}/**
 * @license
 * Copyright 2017 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */const WI=16384,HI=45e3;let _r=null;typeof MozWebSocket<"u"?_r=MozWebSocket:typeof WebSocket<"u"&&(_r=WebSocket);class st{constructor(e,n,s,i,r,o,a){this.connId=e,this.applicationId=s,this.appCheckToken=i,this.authToken=r,this.keepaliveTimer=null,this.frames=null,this.totalFrames=0,this.bytesSent=0,this.bytesReceived=0,this.log_=Ti(this.connId),this.stats_=nl(n),this.connURL=st.connectionURL_(n,o,a,i,s),this.nodeAdmin=n.nodeAdmin}static connectionURL_(e,n,s,i,r){const o={};return o[wf]=tl,typeof location<"u"&&location.hostname&&Sf.test(location.hostname)&&(o[bf]=Cf),n&&(o[If]=n),s&&(o[Tf]=s),i&&(o[oa]=i),r&&(o[Rf]=r),kf(e,Pf,o)}open(e,n){this.onDisconnect=n,this.onMessage=e,this.log_("Websocket connecting to "+this.connURL),this.everConnected_=!1,Nn.set("previous_websocket_failure",!0);try{let s;Pv(),this.mySock=new _r(this.connURL,[],s)}catch(s){this.log_("Error instantiating WebSocket.");const i=s.message||s.data;i&&this.log_(i),this.onClosed_();return}this.mySock.onopen=()=>{this.log_("Websocket connected."),this.everConnected_=!0},this.mySock.onclose=()=>{this.log_("Websocket connection was disconnected."),this.mySock=null,this.onClosed_()},this.mySock.onmessage=s=>{this.handleIncomingFrame(s)},this.mySock.onerror=s=>{this.log_("WebSocket error.  Closing connection.");const i=s.message||s.data;i&&this.log_(i),this.onClosed_()}}start(){}static forceDisallow(){st.forceDisallow_=!0}static isAvailable(){let e=!1;if(typeof navigator<"u"&&navigator.userAgent){const n=/Android ([0-9]{0,}\.[0-9]{0,})/,s=navigator.userAgent.match(n);s&&s.length>1&&parseFloat(s[1])<4.4&&(e=!0)}return!e&&_r!==null&&!st.forceDisallow_}static previouslyFailed(){return Nn.isInMemoryStorage||Nn.get("previous_websocket_failure")===!0}markConnectionHealthy(){Nn.remove("previous_websocket_failure")}appendFrame_(e){if(this.frames.push(e),this.frames.length===this.totalFrames){const n=this.frames.join("");this.frames=null;const s=si(n);this.onMessage(s)}}handleNewFrameCount_(e){this.totalFrames=e,this.frames=[]}extractFrameCount_(e){if(C(this.frames===null,"We already have a frame buffer"),e.length<=6){const n=Number(e);if(!isNaN(n))return this.handleNewFrameCount_(n),null}return this.handleNewFrameCount_(1),e}handleIncomingFrame(e){if(this.mySock===null)return;const n=e.data;if(this.bytesReceived+=n.length,this.stats_.incrementCounter("bytes_received",n.length),this.resetKeepAlive(),this.frames!==null)this.appendFrame_(n);else{const s=this.extractFrameCount_(n);s!==null&&this.appendFrame_(s)}}send(e){this.resetKeepAlive();const n=we(e);this.bytesSent+=n.length,this.stats_.incrementCounter("bytes_sent",n.length);const s=yf(n,WI);s.length>1&&this.sendString_(String(s.length));for(let i=0;i<s.length;i++)this.sendString_(s[i])}shutdown_(){this.isClosed_=!0,this.keepaliveTimer&&(clearInterval(this.keepaliveTimer),this.keepaliveTimer=null),this.mySock&&(this.mySock.close(),this.mySock=null)}onClosed_(){this.isClosed_||(this.log_("WebSocket is closing itself"),this.shutdown_(),this.onDisconnect&&(this.onDisconnect(this.everConnected_),this.onDisconnect=null))}close(){this.isClosed_||(this.log_("WebSocket is being closed"),this.shutdown_())}resetKeepAlive(){clearInterval(this.keepaliveTimer),this.keepaliveTimer=setInterval(()=>{this.mySock&&this.sendString_("0"),this.resetKeepAlive()},Math.floor(HI))}sendString_(e){try{this.mySock.send(e)}catch(n){this.log_("Exception thrown from WebSocket.send():",n.message||n.data,"Closing connection."),setTimeout(this.onClosed_.bind(this),0)}}}st.responsesRequiredToBeHealthy=2;st.healthyTimeout=3e4;/**
 * @license
 * Copyright 2017 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */class ai{constructor(e){this.initTransports_(e)}static get ALL_TRANSPORTS(){return[Xn,st]}static get IS_TRANSPORT_INITIALIZED(){return this.globalTransportInitialized_}initTransports_(e){const n=st&&st.isAvailable();let s=n&&!st.previouslyFailed();if(e.webSocketOnly&&(n||je("wss:// URL used, but browser isn't known to support websockets.  Trying anyway."),s=!0),s)this.transports_=[st];else{const i=this.transports_=[];for(const r of ai.ALL_TRANSPORTS)r&&r.isAvailable()&&i.push(r);ai.globalTransportInitialized_=!0}}initialTransport(){if(this.transports_.length>0)return this.transports_[0];throw new Error("No transports available")}upgradeTransport(){return this.transports_.length>1?this.transports_[1]:null}}ai.globalTransportInitialized_=!1;/**
 * @license
 * Copyright 2017 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */const VI=6e4,BI=5e3,$I=10*1024,jI=100*1024,Ao="t",pu="d",GI="s",_u="r",KI="e",gu="o",mu="a",vu="n",yu="p",qI="h";class zI{constructor(e,n,s,i,r,o,a,l,c,h){this.id=e,this.repoInfo_=n,this.applicationId_=s,this.appCheckToken_=i,this.authToken_=r,this.onMessage_=o,this.onReady_=a,this.onDisconnect_=l,this.onKill_=c,this.lastSessionId=h,this.connectionCount=0,this.pendingDataMessages=[],this.state_=0,this.log_=Ti("c:"+this.id+":"),this.transportManager_=new ai(n),this.log_("Connection created"),this.start_()}start_(){const e=this.transportManager_.initialTransport();this.conn_=new e(this.nextTransportId_(),this.repoInfo_,this.applicationId_,this.appCheckToken_,this.authToken_,null,this.lastSessionId),this.primaryResponsesRequired_=e.responsesRequiredToBeHealthy||0;const n=this.connReceiver_(this.conn_),s=this.disconnReceiver_(this.conn_);this.tx_=this.conn_,this.rx_=this.conn_,this.secondaryConn_=null,this.isHealthy_=!1,setTimeout(()=>{this.conn_&&this.conn_.open(n,s)},Math.floor(0));const i=e.healthyTimeout||0;i>0&&(this.healthyTimeout_=Ks(()=>{this.healthyTimeout_=null,this.isHealthy_||(this.conn_&&this.conn_.bytesReceived>jI?(this.log_("Connection exceeded healthy timeout but has received "+this.conn_.bytesReceived+" bytes.  Marking connection healthy."),this.isHealthy_=!0,this.conn_.markConnectionHealthy()):this.conn_&&this.conn_.bytesSent>$I?this.log_("Connection exceeded healthy timeout but has sent "+this.conn_.bytesSent+" bytes.  Leaving connection alive."):(this.log_("Closing unhealthy connection after timeout."),this.close()))},Math.floor(i)))}nextTransportId_(){return"c:"+this.id+":"+this.connectionCount++}disconnReceiver_(e){return n=>{e===this.conn_?this.onConnectionLost_(n):e===this.secondaryConn_?(this.log_("Secondary connection lost."),this.onSecondaryConnectionLost_()):this.log_("closing an old connection")}}connReceiver_(e){return n=>{this.state_!==2&&(e===this.rx_?this.onPrimaryMessageReceived_(n):e===this.secondaryConn_?this.onSecondaryMessageReceived_(n):this.log_("message on old connection"))}}sendRequest(e){const n={t:"d",d:e};this.sendData_(n)}tryCleanupConnection(){this.tx_===this.secondaryConn_&&this.rx_===this.secondaryConn_&&(this.log_("cleaning up and promoting a connection: "+this.secondaryConn_.connId),this.conn_=this.secondaryConn_,this.secondaryConn_=null)}onSecondaryControl_(e){if(Ao in e){const n=e[Ao];n===mu?this.upgradeIfSecondaryHealthy_():n===_u?(this.log_("Got a reset on secondary, closing it"),this.secondaryConn_.close(),(this.tx_===this.secondaryConn_||this.rx_===this.secondaryConn_)&&this.close()):n===gu&&(this.log_("got pong on secondary."),this.secondaryResponsesRequired_--,this.upgradeIfSecondaryHealthy_())}}onSecondaryMessageReceived_(e){const n=Ps("t",e),s=Ps("d",e);if(n==="c")this.onSecondaryControl_(s);else if(n==="d")this.pendingDataMessages.push(s);else throw new Error("Unknown protocol layer: "+n)}upgradeIfSecondaryHealthy_(){this.secondaryResponsesRequired_<=0?(this.log_("Secondary connection is healthy."),this.isHealthy_=!0,this.secondaryConn_.markConnectionHealthy(),this.proceedWithUpgrade_()):(this.log_("sending ping on secondary."),this.secondaryConn_.send({t:"c",d:{t:yu,d:{}}}))}proceedWithUpgrade_(){this.secondaryConn_.start(),this.log_("sending client ack on secondary"),this.secondaryConn_.send({t:"c",d:{t:mu,d:{}}}),this.log_("Ending transmission on primary"),this.conn_.send({t:"c",d:{t:vu,d:{}}}),this.tx_=this.secondaryConn_,this.tryCleanupConnection()}onPrimaryMessageReceived_(e){const n=Ps("t",e),s=Ps("d",e);n==="c"?this.onControl_(s):n==="d"&&this.onDataMessage_(s)}onDataMessage_(e){this.onPrimaryResponse_(),this.onMessage_(e)}onPrimaryResponse_(){this.isHealthy_||(this.primaryResponsesRequired_--,this.primaryResponsesRequired_<=0&&(this.log_("Primary connection is healthy."),this.isHealthy_=!0,this.conn_.markConnectionHealthy()))}onControl_(e){const n=Ps(Ao,e);if(pu in e){const s=e[pu];if(n===qI){const i=Object.assign({},s);this.repoInfo_.isUsingEmulator&&(i.h=this.repoInfo_.host),this.onHandshake_(i)}else if(n===vu){this.log_("recvd end transmission on primary"),this.rx_=this.secondaryConn_;for(let i=0;i<this.pendingDataMessages.length;++i)this.onDataMessage_(this.pendingDataMessages[i]);this.pendingDataMessages=[],this.tryCleanupConnection()}else n===GI?this.onConnectionShutdown_(s):n===_u?this.onReset_(s):n===KI?ra("Server Error: "+s):n===gu?(this.log_("got pong on primary."),this.onPrimaryResponse_(),this.sendPingOnPrimaryIfNecessary_()):ra("Unknown control packet command: "+n)}}onHandshake_(e){const n=e.ts,s=e.v,i=e.h;this.sessionId=e.s,this.repoInfo_.host=i,this.state_===0&&(this.conn_.start(),this.onConnectionEstablished_(this.conn_,n),tl!==s&&je("Protocol version mismatch detected"),this.tryStartUpgrade_())}tryStartUpgrade_(){const e=this.transportManager_.upgradeTransport();e&&this.startUpgrade_(e)}startUpgrade_(e){this.secondaryConn_=new e(this.nextTransportId_(),this.repoInfo_,this.applicationId_,this.appCheckToken_,this.authToken_,this.sessionId),this.secondaryResponsesRequired_=e.responsesRequiredToBeHealthy||0;const n=this.connReceiver_(this.secondaryConn_),s=this.disconnReceiver_(this.secondaryConn_);this.secondaryConn_.open(n,s),Ks(()=>{this.secondaryConn_&&(this.log_("Timed out trying to upgrade."),this.secondaryConn_.close())},Math.floor(VI))}onReset_(e){this.log_("Reset packet received.  New host: "+e),this.repoInfo_.host=e,this.state_===1?this.close():(this.closeConnections_(),this.start_())}onConnectionEstablished_(e,n){this.log_("Realtime connection established."),this.conn_=e,this.state_=1,this.onReady_&&(this.onReady_(n,this.sessionId),this.onReady_=null),this.primaryResponsesRequired_===0?(this.log_("Primary connection is healthy."),this.isHealthy_=!0):Ks(()=>{this.sendPingOnPrimaryIfNecessary_()},Math.floor(BI))}sendPingOnPrimaryIfNecessary_(){!this.isHealthy_&&this.state_===1&&(this.log_("sending ping on primary."),this.sendData_({t:"c",d:{t:yu,d:{}}}))}onSecondaryConnectionLost_(){const e=this.secondaryConn_;this.secondaryConn_=null,(this.tx_===e||this.rx_===e)&&this.close()}onConnectionLost_(e){this.conn_=null,!e&&this.state_===0?(this.log_("Realtime connection failed."),this.repoInfo_.isCacheableHost()&&(Nn.remove("host:"+this.repoInfo_.host),this.repoInfo_.internalHost=this.repoInfo_.host)):this.state_===1&&this.log_("Realtime connection lost."),this.close()}onConnectionShutdown_(e){this.log_("Connection shutdown command received. Shutting down..."),this.onKill_&&(this.onKill_(e),this.onKill_=null),this.onDisconnect_=null,this.close()}sendData_(e){if(this.state_!==1)throw"Connection is not connected";this.tx_.send(e)}close(){this.state_!==2&&(this.log_("Closing realtime connection."),this.state_=2,this.closeConnections_(),this.onDisconnect_&&(this.onDisconnect_(),this.onDisconnect_=null))}closeConnections_(){this.log_("Shutting down all connections"),this.conn_&&(this.conn_.close(),this.conn_=null),this.secondaryConn_&&(this.secondaryConn_.close(),this.secondaryConn_=null),this.healthyTimeout_&&(clearTimeout(this.healthyTimeout_),this.healthyTimeout_=null)}}/**
 * @license
 * Copyright 2017 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */class Ff{put(e,n,s,i){}merge(e,n,s,i){}refreshAuthToken(e){}refreshAppCheckToken(e){}onDisconnectPut(e,n,s){}onDisconnectMerge(e,n,s){}onDisconnectCancel(e,n){}reportStats(e){}}/**
 * @license
 * Copyright 2017 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */class Uf{constructor(e){this.allowedEvents_=e,this.listeners_={},C(Array.isArray(e)&&e.length>0,"Requires a non-empty array")}trigger(e,...n){if(Array.isArray(this.listeners_[e])){const s=[...this.listeners_[e]];for(let i=0;i<s.length;i++)s[i].callback.apply(s[i].context,n)}}on(e,n,s){this.validateEventType_(e),this.listeners_[e]=this.listeners_[e]||[],this.listeners_[e].push({callback:n,context:s});const i=this.getInitialEvent(e);i&&n.apply(s,i)}off(e,n,s){this.validateEventType_(e);const i=this.listeners_[e]||[];for(let r=0;r<i.length;r++)if(i[r].callback===n&&(!s||s===i[r].context)){i.splice(r,1);return}}validateEventType_(e){C(this.allowedEvents_.find(n=>n===e),"Unknown event: "+e)}}/**
 * @license
 * Copyright 2017 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */class gr extends Uf{constructor(){super(["online"]),this.online_=!0,typeof window<"u"&&typeof window.addEventListener<"u"&&!Ua()&&(window.addEventListener("online",()=>{this.online_||(this.online_=!0,this.trigger("online",!0))},!1),window.addEventListener("offline",()=>{this.online_&&(this.online_=!1,this.trigger("online",!1))},!1))}static getInstance(){return new gr}getInitialEvent(e){return C(e==="online","Unknown event type: "+e),[this.online_]}currentlyOnline(){return this.online_}}/**
 * @license
 * Copyright 2017 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */const Eu=32,wu=768;class le{constructor(e,n){if(n===void 0){this.pieces_=e.split("/");let s=0;for(let i=0;i<this.pieces_.length;i++)this.pieces_[i].length>0&&(this.pieces_[s]=this.pieces_[i],s++);this.pieces_.length=s,this.pieceNum_=0}else this.pieces_=e,this.pieceNum_=n}toString(){let e="";for(let n=this.pieceNum_;n<this.pieces_.length;n++)this.pieces_[n]!==""&&(e+="/"+this.pieces_[n]);return e||"/"}}function ee(){return new le("")}function z(t){return t.pieceNum_>=t.pieces_.length?null:t.pieces_[t.pieceNum_]}function hn(t){return t.pieces_.length-t.pieceNum_}function he(t){let e=t.pieceNum_;return e<t.pieces_.length&&e++,new le(t.pieces_,e)}function Wf(t){return t.pieceNum_<t.pieces_.length?t.pieces_[t.pieces_.length-1]:null}function YI(t){let e="";for(let n=t.pieceNum_;n<t.pieces_.length;n++)t.pieces_[n]!==""&&(e+="/"+encodeURIComponent(String(t.pieces_[n])));return e||"/"}function Hf(t,e=0){return t.pieces_.slice(t.pieceNum_+e)}function Vf(t){if(t.pieceNum_>=t.pieces_.length)return null;const e=[];for(let n=t.pieceNum_;n<t.pieces_.length-1;n++)e.push(t.pieces_[n]);return new le(e,0)}function Ie(t,e){const n=[];for(let s=t.pieceNum_;s<t.pieces_.length;s++)n.push(t.pieces_[s]);if(e instanceof le)for(let s=e.pieceNum_;s<e.pieces_.length;s++)n.push(e.pieces_[s]);else{const s=e.split("/");for(let i=0;i<s.length;i++)s[i].length>0&&n.push(s[i])}return new le(n,0)}function Q(t){return t.pieceNum_>=t.pieces_.length}function Le(t,e){const n=z(t),s=z(e);if(n===null)return e;if(n===s)return Le(he(t),he(e));throw new Error("INTERNAL ERROR: innerPath ("+e+") is not within outerPath ("+t+")")}function il(t,e){if(hn(t)!==hn(e))return!1;for(let n=t.pieceNum_,s=e.pieceNum_;n<=t.pieces_.length;n++,s++)if(t.pieces_[n]!==e.pieces_[s])return!1;return!0}function rt(t,e){let n=t.pieceNum_,s=e.pieceNum_;if(hn(t)>hn(e))return!1;for(;n<t.pieces_.length;){if(t.pieces_[n]!==e.pieces_[s])return!1;++n,++s}return!0}class QI{constructor(e,n){this.errorPrefix_=n,this.parts_=Hf(e,0),this.byteLength_=Math.max(1,this.parts_.length);for(let s=0;s<this.parts_.length;s++)this.byteLength_+=Kr(this.parts_[s]);Bf(this)}}function JI(t,e){t.parts_.length>0&&(t.byteLength_+=1),t.parts_.push(e),t.byteLength_+=Kr(e),Bf(t)}function XI(t){const e=t.parts_.pop();t.byteLength_-=Kr(e),t.parts_.length>0&&(t.byteLength_-=1)}function Bf(t){if(t.byteLength_>wu)throw new Error(t.errorPrefix_+"has a key path longer than "+wu+" bytes ("+t.byteLength_+").");if(t.parts_.length>Eu)throw new Error(t.errorPrefix_+"path specified exceeds the maximum depth that can be written ("+Eu+") or object contains a cycle "+Rn(t))}function Rn(t){return t.parts_.length===0?"":"in property '"+t.parts_.join(".")+"'"}/**
 * @license
 * Copyright 2017 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */class rl extends Uf{constructor(){super(["visible"]);let e,n;typeof document<"u"&&typeof document.addEventListener<"u"&&(typeof document.hidden<"u"?(n="visibilitychange",e="hidden"):typeof document.mozHidden<"u"?(n="mozvisibilitychange",e="mozHidden"):typeof document.msHidden<"u"?(n="msvisibilitychange",e="msHidden"):typeof document.webkitHidden<"u"&&(n="webkitvisibilitychange",e="webkitHidden")),this.visible_=!0,n&&document.addEventListener(n,()=>{const s=!document[e];s!==this.visible_&&(this.visible_=s,this.trigger("visible",s))},!1)}static getInstance(){return new rl}getInitialEvent(e){return C(e==="visible","Unknown event type: "+e),[this.visible_]}}/**
 * @license
 * Copyright 2017 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */const As=1e3,ZI=60*5*1e3,Iu=30*1e3,eb=1.3,tb=3e4,nb="server_kill",bu=3;class Lt extends Ff{constructor(e,n,s,i,r,o,a,l){if(super(),this.repoInfo_=e,this.applicationId_=n,this.onDataUpdate_=s,this.onConnectStatus_=i,this.onServerInfoUpdate_=r,this.authTokenProvider_=o,this.appCheckTokenProvider_=a,this.authOverride_=l,this.id=Lt.nextPersistentConnectionId_++,this.log_=Ti("p:"+this.id+":"),this.interruptReasons_={},this.listens=new Map,this.outstandingPuts_=[],this.outstandingGets_=[],this.outstandingPutCount_=0,this.outstandingGetCount_=0,this.onDisconnectRequestQueue_=[],this.connected_=!1,this.reconnectDelay_=As,this.maxReconnectDelay_=ZI,this.securityDebugCallback_=null,this.lastSessionId=null,this.establishConnectionTimer_=null,this.visible_=!1,this.requestCBHash_={},this.requestNumber_=0,this.realtime_=null,this.authToken_=null,this.appCheckToken_=null,this.forceTokenRefresh_=!1,this.invalidAuthTokenCount_=0,this.invalidAppCheckTokenCount_=0,this.firstConnection_=!0,this.lastConnectionAttemptTime_=null,this.lastConnectionEstablishedTime_=null,l)throw new Error("Auth override specified in options, but not supported on non Node.js platforms");rl.getInstance().on("visible",this.onVisible_,this),e.host.indexOf("fblocal")===-1&&gr.getInstance().on("online",this.onOnline_,this)}sendRequest(e,n,s){const i=++this.requestNumber_,r={r:i,a:e,b:n};this.log_(we(r)),C(this.connected_,"sendRequest call when we're not connected not allowed."),this.realtime_.sendRequest(r),s&&(this.requestCBHash_[i]=s)}get(e){this.initConnection_();const n=new Gr,i={action:"g",request:{p:e._path.toString(),q:e._queryObject},onComplete:o=>{const a=o.d;o.s==="ok"?n.resolve(a):n.reject(a)}};this.outstandingGets_.push(i),this.outstandingGetCount_++;const r=this.outstandingGets_.length-1;return this.connected_&&this.sendGet_(r),n.promise}listen(e,n,s,i){this.initConnection_();const r=e._queryIdentifier,o=e._path.toString();this.log_("Listen called for "+o+" "+r),this.listens.has(o)||this.listens.set(o,new Map),C(e._queryParams.isDefault()||!e._queryParams.loadsAllData(),"listen() called for non-default but complete query"),C(!this.listens.get(o).has(r),"listen() called twice for same path/queryId.");const a={onComplete:i,hashFn:n,query:e,tag:s};this.listens.get(o).set(r,a),this.connected_&&this.sendListen_(a)}sendGet_(e){const n=this.outstandingGets_[e];this.sendRequest("g",n.request,s=>{delete this.outstandingGets_[e],this.outstandingGetCount_--,this.outstandingGetCount_===0&&(this.outstandingGets_=[]),n.onComplete&&n.onComplete(s)})}sendListen_(e){const n=e.query,s=n._path.toString(),i=n._queryIdentifier;this.log_("Listen on "+s+" for "+i);const r={p:s},o="q";e.tag&&(r.q=n._queryObject,r.t=e.tag),r.h=e.hashFn(),this.sendRequest(o,r,a=>{const l=a.d,c=a.s;Lt.warnOnListenWarnings_(l,n),(this.listens.get(s)&&this.listens.get(s).get(i))===e&&(this.log_("listen response",a),c!=="ok"&&this.removeListen_(s,i),e.onComplete&&e.onComplete(c,l))})}static warnOnListenWarnings_(e,n){if(e&&typeof e=="object"&&Vt(e,"w")){const s=ds(e,"w");if(Array.isArray(s)&&~s.indexOf("no_index")){const i='".indexOn": "'+n._queryParams.getIndex().toString()+'"',r=n._path.toString();je(`Using an unspecified index. Your data will be downloaded and filtered on the client. Consider adding ${i} at ${r} to your security rules for better performance.`)}}}refreshAuthToken(e){this.authToken_=e,this.log_("Auth token refreshed"),this.authToken_?this.tryAuth():this.connected_&&this.sendRequest("unauth",{},()=>{}),this.reduceReconnectDelayIfAdminCredential_(e)}reduceReconnectDelayIfAdminCredential_(e){(e&&e.length===40||Mv(e))&&(this.log_("Admin auth credential detected.  Reducing max reconnect time."),this.maxReconnectDelay_=Iu)}refreshAppCheckToken(e){this.appCheckToken_=e,this.log_("App check token refreshed"),this.appCheckToken_?this.tryAppCheck():this.connected_&&this.sendRequest("unappeck",{},()=>{})}tryAuth(){if(this.connected_&&this.authToken_){const e=this.authToken_,n=Dv(e)?"auth":"gauth",s={cred:e};this.authOverride_===null?s.noauth=!0:typeof this.authOverride_=="object"&&(s.authvar=this.authOverride_),this.sendRequest(n,s,i=>{const r=i.s,o=i.d||"error";this.authToken_===e&&(r==="ok"?this.invalidAuthTokenCount_=0:this.onAuthRevoked_(r,o))})}}tryAppCheck(){this.connected_&&this.appCheckToken_&&this.sendRequest("appcheck",{token:this.appCheckToken_},e=>{const n=e.s,s=e.d||"error";n==="ok"?this.invalidAppCheckTokenCount_=0:this.onAppCheckRevoked_(n,s)})}unlisten(e,n){const s=e._path.toString(),i=e._queryIdentifier;this.log_("Unlisten called for "+s+" "+i),C(e._queryParams.isDefault()||!e._queryParams.loadsAllData(),"unlisten() called for non-default but complete query"),this.removeListen_(s,i)&&this.connected_&&this.sendUnlisten_(s,i,e._queryObject,n)}sendUnlisten_(e,n,s,i){this.log_("Unlisten on "+e+" for "+n);const r={p:e},o="n";i&&(r.q=s,r.t=i),this.sendRequest(o,r)}onDisconnectPut(e,n,s){this.initConnection_(),this.connected_?this.sendOnDisconnect_("o",e,n,s):this.onDisconnectRequestQueue_.push({pathString:e,action:"o",data:n,onComplete:s})}onDisconnectMerge(e,n,s){this.initConnection_(),this.connected_?this.sendOnDisconnect_("om",e,n,s):this.onDisconnectRequestQueue_.push({pathString:e,action:"om",data:n,onComplete:s})}onDisconnectCancel(e,n){this.initConnection_(),this.connected_?this.sendOnDisconnect_("oc",e,null,n):this.onDisconnectRequestQueue_.push({pathString:e,action:"oc",data:null,onComplete:n})}sendOnDisconnect_(e,n,s,i){const r={p:n,d:s};this.log_("onDisconnect "+e,r),this.sendRequest(e,r,o=>{i&&setTimeout(()=>{i(o.s,o.d)},Math.floor(0))})}put(e,n,s,i){this.putInternal("p",e,n,s,i)}merge(e,n,s,i){this.putInternal("m",e,n,s,i)}putInternal(e,n,s,i,r){this.initConnection_();const o={p:n,d:s};r!==void 0&&(o.h=r),this.outstandingPuts_.push({action:e,request:o,onComplete:i}),this.outstandingPutCount_++;const a=this.outstandingPuts_.length-1;this.connected_?this.sendPut_(a):this.log_("Buffering put: "+n)}sendPut_(e){const n=this.outstandingPuts_[e].action,s=this.outstandingPuts_[e].request,i=this.outstandingPuts_[e].onComplete;this.outstandingPuts_[e].queued=this.connected_,this.sendRequest(n,s,r=>{this.log_(n+" response",r),delete this.outstandingPuts_[e],this.outstandingPutCount_--,this.outstandingPutCount_===0&&(this.outstandingPuts_=[]),i&&i(r.s,r.d)})}reportStats(e){if(this.connected_){const n={c:e};this.log_("reportStats",n),this.sendRequest("s",n,s=>{if(s.s!=="ok"){const r=s.d;this.log_("reportStats","Error sending stats: "+r)}})}}onDataMessage_(e){if("r"in e){this.log_("from server: "+we(e));const n=e.r,s=this.requestCBHash_[n];s&&(delete this.requestCBHash_[n],s(e.b))}else{if("error"in e)throw"A server-side error has occurred: "+e.error;"a"in e&&this.onDataPush_(e.a,e.b)}}onDataPush_(e,n){this.log_("handleServerMessage",e,n),e==="d"?this.onDataUpdate_(n.p,n.d,!1,n.t):e==="m"?this.onDataUpdate_(n.p,n.d,!0,n.t):e==="c"?this.onListenRevoked_(n.p,n.q):e==="ac"?this.onAuthRevoked_(n.s,n.d):e==="apc"?this.onAppCheckRevoked_(n.s,n.d):e==="sd"?this.onSecurityDebugPacket_(n):ra("Unrecognized action received from server: "+we(e)+`
Are you using the latest client?`)}onReady_(e,n){this.log_("connection ready"),this.connected_=!0,this.lastConnectionEstablishedTime_=new Date().getTime(),this.handleTimestamp_(e),this.lastSessionId=n,this.firstConnection_&&this.sendConnectStats_(),this.restoreState_(),this.firstConnection_=!1,this.onConnectStatus_(!0)}scheduleConnect_(e){C(!this.realtime_,"Scheduling a connect when we're already connected/ing?"),this.establishConnectionTimer_&&clearTimeout(this.establishConnectionTimer_),this.establishConnectionTimer_=setTimeout(()=>{this.establishConnectionTimer_=null,this.establishConnection_()},Math.floor(e))}initConnection_(){!this.realtime_&&this.firstConnection_&&this.scheduleConnect_(0)}onVisible_(e){e&&!this.visible_&&this.reconnectDelay_===this.maxReconnectDelay_&&(this.log_("Window became visible.  Reducing delay."),this.reconnectDelay_=As,this.realtime_||this.scheduleConnect_(0)),this.visible_=e}onOnline_(e){e?(this.log_("Browser went online."),this.reconnectDelay_=As,this.realtime_||this.scheduleConnect_(0)):(this.log_("Browser went offline.  Killing connection."),this.realtime_&&this.realtime_.close())}onRealtimeDisconnect_(){if(this.log_("data client disconnected"),this.connected_=!1,this.realtime_=null,this.cancelSentTransactions_(),this.requestCBHash_={},this.shouldReconnect_()){this.visible_?this.lastConnectionEstablishedTime_&&(new Date().getTime()-this.lastConnectionEstablishedTime_>tb&&(this.reconnectDelay_=As),this.lastConnectionEstablishedTime_=null):(this.log_("Window isn't visible.  Delaying reconnect."),this.reconnectDelay_=this.maxReconnectDelay_,this.lastConnectionAttemptTime_=new Date().getTime());const e=new Date().getTime()-this.lastConnectionAttemptTime_;let n=Math.max(0,this.reconnectDelay_-e);n=Math.random()*n,this.log_("Trying to reconnect in "+n+"ms"),this.scheduleConnect_(n),this.reconnectDelay_=Math.min(this.maxReconnectDelay_,this.reconnectDelay_*eb)}this.onConnectStatus_(!1)}async establishConnection_(){if(this.shouldReconnect_()){this.log_("Making a connection attempt"),this.lastConnectionAttemptTime_=new Date().getTime(),this.lastConnectionEstablishedTime_=null;const e=this.onDataMessage_.bind(this),n=this.onReady_.bind(this),s=this.onRealtimeDisconnect_.bind(this),i=this.id+":"+Lt.nextConnectionId_++,r=this.lastSessionId;let o=!1,a=null;const l=function(){a?a.close():(o=!0,s())},c=function(u){C(a,"sendRequest call when we're not connected not allowed."),a.sendRequest(u)};this.realtime_={close:l,sendRequest:c};const h=this.forceTokenRefresh_;this.forceTokenRefresh_=!1;try{const[u,d]=await Promise.all([this.authTokenProvider_.getToken(h),this.appCheckTokenProvider_.getToken(h)]);o?Ne("getToken() completed but was canceled"):(Ne("getToken() completed. Creating connection."),this.authToken_=u&&u.accessToken,this.appCheckToken_=d&&d.token,a=new zI(i,this.repoInfo_,this.applicationId_,this.appCheckToken_,this.authToken_,e,n,s,_=>{je(_+" ("+this.repoInfo_.toString()+")"),this.interrupt(nb)},r))}catch(u){this.log_("Failed to get token: "+u),o||(this.repoInfo_.nodeAdmin&&je(u),l())}}}interrupt(e){Ne("Interrupting connection for reason: "+e),this.interruptReasons_[e]=!0,this.realtime_?this.realtime_.close():(this.establishConnectionTimer_&&(clearTimeout(this.establishConnectionTimer_),this.establishConnectionTimer_=null),this.connected_&&this.onRealtimeDisconnect_())}resume(e){Ne("Resuming connection for reason: "+e),delete this.interruptReasons_[e],Yo(this.interruptReasons_)&&(this.reconnectDelay_=As,this.realtime_||this.scheduleConnect_(0))}handleTimestamp_(e){const n=e-new Date().getTime();this.onServerInfoUpdate_({serverTimeOffset:n})}cancelSentTransactions_(){for(let e=0;e<this.outstandingPuts_.length;e++){const n=this.outstandingPuts_[e];n&&"h"in n.request&&n.queued&&(n.onComplete&&n.onComplete("disconnect"),delete this.outstandingPuts_[e],this.outstandingPutCount_--)}this.outstandingPutCount_===0&&(this.outstandingPuts_=[])}onListenRevoked_(e,n){let s;n?s=n.map(r=>el(r)).join("$"):s="default";const i=this.removeListen_(e,s);i&&i.onComplete&&i.onComplete("permission_denied")}removeListen_(e,n){const s=new le(e).toString();let i;if(this.listens.has(s)){const r=this.listens.get(s);i=r.get(n),r.delete(n),r.size===0&&this.listens.delete(s)}else i=void 0;return i}onAuthRevoked_(e,n){Ne("Auth token revoked: "+e+"/"+n),this.authToken_=null,this.forceTokenRefresh_=!0,this.realtime_.close(),(e==="invalid_token"||e==="permission_denied")&&(this.invalidAuthTokenCount_++,this.invalidAuthTokenCount_>=bu&&(this.reconnectDelay_=Iu,this.authTokenProvider_.notifyForInvalidToken()))}onAppCheckRevoked_(e,n){Ne("App check token revoked: "+e+"/"+n),this.appCheckToken_=null,this.forceTokenRefresh_=!0,(e==="invalid_token"||e==="permission_denied")&&(this.invalidAppCheckTokenCount_++,this.invalidAppCheckTokenCount_>=bu&&this.appCheckTokenProvider_.notifyForInvalidToken())}onSecurityDebugPacket_(e){this.securityDebugCallback_?this.securityDebugCallback_(e):"msg"in e&&console.log("FIREBASE: "+e.msg.replace(`
`,`
FIREBASE: `))}restoreState_(){this.tryAuth(),this.tryAppCheck();for(const e of this.listens.values())for(const n of e.values())this.sendListen_(n);for(let e=0;e<this.outstandingPuts_.length;e++)this.outstandingPuts_[e]&&this.sendPut_(e);for(;this.onDisconnectRequestQueue_.length;){const e=this.onDisconnectRequestQueue_.shift();this.sendOnDisconnect_(e.action,e.pathString,e.data,e.onComplete)}for(let e=0;e<this.outstandingGets_.length;e++)this.outstandingGets_[e]&&this.sendGet_(e)}sendConnectStats_(){const e={};let n="js";e["sdk."+n+"."+_f.replace(/\./g,"-")]=1,Ua()?e["framework.cordova"]=1:Rd()&&(e["framework.reactnative"]=1),this.reportStats(e)}shouldReconnect_(){const e=gr.getInstance().currentlyOnline();return Yo(this.interruptReasons_)&&e}}Lt.nextPersistentConnectionId_=0;Lt.nextConnectionId_=0;/**
 * @license
 * Copyright 2017 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */class Y{constructor(e,n){this.name=e,this.node=n}static Wrap(e,n){return new Y(e,n)}}/**
 * @license
 * Copyright 2017 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */class Qr{getCompare(){return this.compare.bind(this)}indexedValueChanged(e,n){const s=new Y(ps,e),i=new Y(ps,n);return this.compare(s,i)!==0}minPost(){return Y.MIN}}/**
 * @license
 * Copyright 2017 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */let Li;class $f extends Qr{static get __EMPTY_NODE(){return Li}static set __EMPTY_NODE(e){Li=e}compare(e,n){return ws(e.name,n.name)}isDefinedOn(e){throw ms("KeyIndex.isDefinedOn not expected to be called.")}indexedValueChanged(e,n){return!1}minPost(){return Y.MIN}maxPost(){return new Y(Wn,Li)}makePost(e,n){return C(typeof e=="string","KeyIndex indexValue must always be a string."),new Y(e,Li)}toString(){return".key"}}const ls=new $f;/**
 * @license
 * Copyright 2017 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */class Fi{constructor(e,n,s,i,r=null){this.isReverse_=i,this.resultGenerator_=r,this.nodeStack_=[];let o=1;for(;!e.isEmpty();)if(e=e,o=n?s(e.key,n):1,i&&(o*=-1),o<0)this.isReverse_?e=e.left:e=e.right;else if(o===0){this.nodeStack_.push(e);break}else this.nodeStack_.push(e),this.isReverse_?e=e.right:e=e.left}getNext(){if(this.nodeStack_.length===0)return null;let e=this.nodeStack_.pop(),n;if(this.resultGenerator_?n=this.resultGenerator_(e.key,e.value):n={key:e.key,value:e.value},this.isReverse_)for(e=e.left;!e.isEmpty();)this.nodeStack_.push(e),e=e.right;else for(e=e.right;!e.isEmpty();)this.nodeStack_.push(e),e=e.left;return n}hasNext(){return this.nodeStack_.length>0}peek(){if(this.nodeStack_.length===0)return null;const e=this.nodeStack_[this.nodeStack_.length-1];return this.resultGenerator_?this.resultGenerator_(e.key,e.value):{key:e.key,value:e.value}}}class Se{constructor(e,n,s,i,r){this.key=e,this.value=n,this.color=s??Se.RED,this.left=i??$e.EMPTY_NODE,this.right=r??$e.EMPTY_NODE}copy(e,n,s,i,r){return new Se(e??this.key,n??this.value,s??this.color,i??this.left,r??this.right)}count(){return this.left.count()+1+this.right.count()}isEmpty(){return!1}inorderTraversal(e){return this.left.inorderTraversal(e)||!!e(this.key,this.value)||this.right.inorderTraversal(e)}reverseTraversal(e){return this.right.reverseTraversal(e)||e(this.key,this.value)||this.left.reverseTraversal(e)}min_(){return this.left.isEmpty()?this:this.left.min_()}minKey(){return this.min_().key}maxKey(){return this.right.isEmpty()?this.key:this.right.maxKey()}insert(e,n,s){let i=this;const r=s(e,i.key);return r<0?i=i.copy(null,null,null,i.left.insert(e,n,s),null):r===0?i=i.copy(null,n,null,null,null):i=i.copy(null,null,null,null,i.right.insert(e,n,s)),i.fixUp_()}removeMin_(){if(this.left.isEmpty())return $e.EMPTY_NODE;let e=this;return!e.left.isRed_()&&!e.left.left.isRed_()&&(e=e.moveRedLeft_()),e=e.copy(null,null,null,e.left.removeMin_(),null),e.fixUp_()}remove(e,n){let s,i;if(s=this,n(e,s.key)<0)!s.left.isEmpty()&&!s.left.isRed_()&&!s.left.left.isRed_()&&(s=s.moveRedLeft_()),s=s.copy(null,null,null,s.left.remove(e,n),null);else{if(s.left.isRed_()&&(s=s.rotateRight_()),!s.right.isEmpty()&&!s.right.isRed_()&&!s.right.left.isRed_()&&(s=s.moveRedRight_()),n(e,s.key)===0){if(s.right.isEmpty())return $e.EMPTY_NODE;i=s.right.min_(),s=s.copy(i.key,i.value,null,null,s.right.removeMin_())}s=s.copy(null,null,null,null,s.right.remove(e,n))}return s.fixUp_()}isRed_(){return this.color}fixUp_(){let e=this;return e.right.isRed_()&&!e.left.isRed_()&&(e=e.rotateLeft_()),e.left.isRed_()&&e.left.left.isRed_()&&(e=e.rotateRight_()),e.left.isRed_()&&e.right.isRed_()&&(e=e.colorFlip_()),e}moveRedLeft_(){let e=this.colorFlip_();return e.right.left.isRed_()&&(e=e.copy(null,null,null,null,e.right.rotateRight_()),e=e.rotateLeft_(),e=e.colorFlip_()),e}moveRedRight_(){let e=this.colorFlip_();return e.left.left.isRed_()&&(e=e.rotateRight_(),e=e.colorFlip_()),e}rotateLeft_(){const e=this.copy(null,null,Se.RED,null,this.right.left);return this.right.copy(null,null,this.color,e,null)}rotateRight_(){const e=this.copy(null,null,Se.RED,this.left.right,null);return this.left.copy(null,null,this.color,null,e)}colorFlip_(){const e=this.left.copy(null,null,!this.left.color,null,null),n=this.right.copy(null,null,!this.right.color,null,null);return this.copy(null,null,!this.color,e,n)}checkMaxDepth_(){const e=this.check_();return Math.pow(2,e)<=this.count()+1}check_(){if(this.isRed_()&&this.left.isRed_())throw new Error("Red node has red child("+this.key+","+this.value+")");if(this.right.isRed_())throw new Error("Right child of ("+this.key+","+this.value+") is red");const e=this.left.check_();if(e!==this.right.check_())throw new Error("Black depths differ");return e+(this.isRed_()?0:1)}}Se.RED=!0;Se.BLACK=!1;class sb{copy(e,n,s,i,r){return this}insert(e,n,s){return new Se(e,n,null)}remove(e,n){return this}count(){return 0}isEmpty(){return!0}inorderTraversal(e){return!1}reverseTraversal(e){return!1}minKey(){return null}maxKey(){return null}check_(){return 0}isRed_(){return!1}}class $e{constructor(e,n=$e.EMPTY_NODE){this.comparator_=e,this.root_=n}insert(e,n){return new $e(this.comparator_,this.root_.insert(e,n,this.comparator_).copy(null,null,Se.BLACK,null,null))}remove(e){return new $e(this.comparator_,this.root_.remove(e,this.comparator_).copy(null,null,Se.BLACK,null,null))}get(e){let n,s=this.root_;for(;!s.isEmpty();){if(n=this.comparator_(e,s.key),n===0)return s.value;n<0?s=s.left:n>0&&(s=s.right)}return null}getPredecessorKey(e){let n,s=this.root_,i=null;for(;!s.isEmpty();)if(n=this.comparator_(e,s.key),n===0){if(s.left.isEmpty())return i?i.key:null;for(s=s.left;!s.right.isEmpty();)s=s.right;return s.key}else n<0?s=s.left:n>0&&(i=s,s=s.right);throw new Error("Attempted to find predecessor key for a nonexistent key.  What gives?")}isEmpty(){return this.root_.isEmpty()}count(){return this.root_.count()}minKey(){return this.root_.minKey()}maxKey(){return this.root_.maxKey()}inorderTraversal(e){return this.root_.inorderTraversal(e)}reverseTraversal(e){return this.root_.reverseTraversal(e)}getIterator(e){return new Fi(this.root_,null,this.comparator_,!1,e)}getIteratorFrom(e,n){return new Fi(this.root_,e,this.comparator_,!1,n)}getReverseIteratorFrom(e,n){return new Fi(this.root_,e,this.comparator_,!0,n)}getReverseIterator(e){return new Fi(this.root_,null,this.comparator_,!0,e)}}$e.EMPTY_NODE=new sb;/**
 * @license
 * Copyright 2017 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */function ib(t,e){return ws(t.name,e.name)}function ol(t,e){return ws(t,e)}/**
 * @license
 * Copyright 2017 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */let aa;function rb(t){aa=t}const jf=function(t){return typeof t=="number"?"number:"+Ef(t):"string:"+t},Gf=function(t){if(t.isLeafNode()){const e=t.val();C(typeof e=="string"||typeof e=="number"||typeof e=="object"&&Vt(e,".sv"),"Priority must be a string or number.")}else C(t===aa||t.isEmpty(),"priority of unexpected type.");C(t===aa||t.getPriority().isEmpty(),"Priority nodes can't have a priority of their own.")};/**
 * @license
 * Copyright 2017 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */let Cu;class Ce{constructor(e,n=Ce.__childrenNodeConstructor.EMPTY_NODE){this.value_=e,this.priorityNode_=n,this.lazyHash_=null,C(this.value_!==void 0&&this.value_!==null,"LeafNode shouldn't be created with null/undefined value."),Gf(this.priorityNode_)}static set __childrenNodeConstructor(e){Cu=e}static get __childrenNodeConstructor(){return Cu}isLeafNode(){return!0}getPriority(){return this.priorityNode_}updatePriority(e){return new Ce(this.value_,e)}getImmediateChild(e){return e===".priority"?this.priorityNode_:Ce.__childrenNodeConstructor.EMPTY_NODE}getChild(e){return Q(e)?this:z(e)===".priority"?this.priorityNode_:Ce.__childrenNodeConstructor.EMPTY_NODE}hasChild(){return!1}getPredecessorChildName(e,n){return null}updateImmediateChild(e,n){return e===".priority"?this.updatePriority(n):n.isEmpty()&&e!==".priority"?this:Ce.__childrenNodeConstructor.EMPTY_NODE.updateImmediateChild(e,n).updatePriority(this.priorityNode_)}updateChild(e,n){const s=z(e);return s===null?n:n.isEmpty()&&s!==".priority"?this:(C(s!==".priority"||hn(e)===1,".priority must be the last token in a path"),this.updateImmediateChild(s,Ce.__childrenNodeConstructor.EMPTY_NODE.updateChild(he(e),n)))}isEmpty(){return!1}numChildren(){return 0}forEachChild(e,n){return!1}val(e){return e&&!this.getPriority().isEmpty()?{".value":this.getValue(),".priority":this.getPriority().val()}:this.getValue()}hash(){if(this.lazyHash_===null){let e="";this.priorityNode_.isEmpty()||(e+="priority:"+jf(this.priorityNode_.val())+":");const n=typeof this.value_;e+=n+":",n==="number"?e+=Ef(this.value_):e+=this.value_,this.lazyHash_=mf(e)}return this.lazyHash_}getValue(){return this.value_}compareTo(e){return e===Ce.__childrenNodeConstructor.EMPTY_NODE?1:e instanceof Ce.__childrenNodeConstructor?-1:(C(e.isLeafNode(),"Unknown node type"),this.compareToLeafNode_(e))}compareToLeafNode_(e){const n=typeof e.value_,s=typeof this.value_,i=Ce.VALUE_TYPE_ORDER.indexOf(n),r=Ce.VALUE_TYPE_ORDER.indexOf(s);return C(i>=0,"Unknown leaf type: "+n),C(r>=0,"Unknown leaf type: "+s),i===r?s==="object"?0:this.value_<e.value_?-1:this.value_===e.value_?0:1:r-i}withIndex(){return this}isIndexed(){return!0}equals(e){if(e===this)return!0;if(e.isLeafNode()){const n=e;return this.value_===n.value_&&this.priorityNode_.equals(n.priorityNode_)}else return!1}}Ce.VALUE_TYPE_ORDER=["object","boolean","number","string"];/**
 * @license
 * Copyright 2017 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */let Kf,qf;function ob(t){Kf=t}function ab(t){qf=t}class lb extends Qr{compare(e,n){const s=e.node.getPriority(),i=n.node.getPriority(),r=s.compareTo(i);return r===0?ws(e.name,n.name):r}isDefinedOn(e){return!e.getPriority().isEmpty()}indexedValueChanged(e,n){return!e.getPriority().equals(n.getPriority())}minPost(){return Y.MIN}maxPost(){return new Y(Wn,new Ce("[PRIORITY-POST]",qf))}makePost(e,n){const s=Kf(e);return new Y(n,new Ce("[PRIORITY-POST]",s))}toString(){return".priority"}}const me=new lb;/**
 * @license
 * Copyright 2017 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */const cb=Math.log(2);class ub{constructor(e){const n=r=>parseInt(Math.log(r)/cb,10),s=r=>parseInt(Array(r+1).join("1"),2);this.count=n(e+1),this.current_=this.count-1;const i=s(this.count);this.bits_=e+1&i}nextBitIsOne(){const e=!(this.bits_&1<<this.current_);return this.current_--,e}}const mr=function(t,e,n,s){t.sort(e);const i=function(l,c){const h=c-l;let u,d;if(h===0)return null;if(h===1)return u=t[l],d=n?n(u):u,new Se(d,u.node,Se.BLACK,null,null);{const _=parseInt(h/2,10)+l,m=i(l,_),I=i(_+1,c);return u=t[_],d=n?n(u):u,new Se(d,u.node,Se.BLACK,m,I)}},r=function(l){let c=null,h=null,u=t.length;const d=function(m,I){const O=u-m,D=u;u-=m;const k=i(O+1,D),M=t[O],x=n?n(M):M;_(new Se(x,M.node,I,null,k))},_=function(m){c?(c.left=m,c=m):(h=m,c=m)};for(let m=0;m<l.count;++m){const I=l.nextBitIsOne(),O=Math.pow(2,l.count-(m+1));I?d(O,Se.BLACK):(d(O,Se.BLACK),d(O,Se.RED))}return h},o=new ub(t.length),a=r(o);return new $e(s||e,a)};/**
 * @license
 * Copyright 2017 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */let No;const Yn={};class Dt{constructor(e,n){this.indexes_=e,this.indexSet_=n}static get Default(){return C(Yn&&me,"ChildrenNode.ts has not been loaded"),No=No||new Dt({".priority":Yn},{".priority":me}),No}get(e){const n=ds(this.indexes_,e);if(!n)throw new Error("No index defined for "+e);return n instanceof $e?n:null}hasIndex(e){return Vt(this.indexSet_,e.toString())}addIndex(e,n){C(e!==ls,"KeyIndex always exists and isn't meant to be added to the IndexMap.");const s=[];let i=!1;const r=n.getIterator(Y.Wrap);let o=r.getNext();for(;o;)i=i||e.isDefinedOn(o.node),s.push(o),o=r.getNext();let a;i?a=mr(s,e.getCompare()):a=Yn;const l=e.toString(),c=Object.assign({},this.indexSet_);c[l]=e;const h=Object.assign({},this.indexes_);return h[l]=a,new Dt(h,c)}addToIndexes(e,n){const s=ar(this.indexes_,(i,r)=>{const o=ds(this.indexSet_,r);if(C(o,"Missing index implementation for "+r),i===Yn)if(o.isDefinedOn(e.node)){const a=[],l=n.getIterator(Y.Wrap);let c=l.getNext();for(;c;)c.name!==e.name&&a.push(c),c=l.getNext();return a.push(e),mr(a,o.getCompare())}else return Yn;else{const a=n.get(e.name);let l=i;return a&&(l=l.remove(new Y(e.name,a))),l.insert(e,e.node)}});return new Dt(s,this.indexSet_)}removeFromIndexes(e,n){const s=ar(this.indexes_,i=>{if(i===Yn)return i;{const r=n.get(e.name);return r?i.remove(new Y(e.name,r)):i}});return new Dt(s,this.indexSet_)}}/**
 * @license
 * Copyright 2017 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */let Ns;class W{constructor(e,n,s){this.children_=e,this.priorityNode_=n,this.indexMap_=s,this.lazyHash_=null,this.priorityNode_&&Gf(this.priorityNode_),this.children_.isEmpty()&&C(!this.priorityNode_||this.priorityNode_.isEmpty(),"An empty node cannot have a priority")}static get EMPTY_NODE(){return Ns||(Ns=new W(new $e(ol),null,Dt.Default))}isLeafNode(){return!1}getPriority(){return this.priorityNode_||Ns}updatePriority(e){return this.children_.isEmpty()?this:new W(this.children_,e,this.indexMap_)}getImmediateChild(e){if(e===".priority")return this.getPriority();{const n=this.children_.get(e);return n===null?Ns:n}}getChild(e){const n=z(e);return n===null?this:this.getImmediateChild(n).getChild(he(e))}hasChild(e){return this.children_.get(e)!==null}updateImmediateChild(e,n){if(C(n,"We should always be passing snapshot nodes"),e===".priority")return this.updatePriority(n);{const s=new Y(e,n);let i,r;n.isEmpty()?(i=this.children_.remove(e),r=this.indexMap_.removeFromIndexes(s,this.children_)):(i=this.children_.insert(e,n),r=this.indexMap_.addToIndexes(s,this.children_));const o=i.isEmpty()?Ns:this.priorityNode_;return new W(i,o,r)}}updateChild(e,n){const s=z(e);if(s===null)return n;{C(z(e)!==".priority"||hn(e)===1,".priority must be the last token in a path");const i=this.getImmediateChild(s).updateChild(he(e),n);return this.updateImmediateChild(s,i)}}isEmpty(){return this.children_.isEmpty()}numChildren(){return this.children_.count()}val(e){if(this.isEmpty())return null;const n={};let s=0,i=0,r=!0;if(this.forEachChild(me,(o,a)=>{n[o]=a.val(e),s++,r&&W.INTEGER_REGEXP_.test(o)?i=Math.max(i,Number(o)):r=!1}),!e&&r&&i<2*s){const o=[];for(const a in n)o[a]=n[a];return o}else return e&&!this.getPriority().isEmpty()&&(n[".priority"]=this.getPriority().val()),n}hash(){if(this.lazyHash_===null){let e="";this.getPriority().isEmpty()||(e+="priority:"+jf(this.getPriority().val())+":"),this.forEachChild(me,(n,s)=>{const i=s.hash();i!==""&&(e+=":"+n+":"+i)}),this.lazyHash_=e===""?"":mf(e)}return this.lazyHash_}getPredecessorChildName(e,n,s){const i=this.resolveIndex_(s);if(i){const r=i.getPredecessorKey(new Y(e,n));return r?r.name:null}else return this.children_.getPredecessorKey(e)}getFirstChildName(e){const n=this.resolveIndex_(e);if(n){const s=n.minKey();return s&&s.name}else return this.children_.minKey()}getFirstChild(e){const n=this.getFirstChildName(e);return n?new Y(n,this.children_.get(n)):null}getLastChildName(e){const n=this.resolveIndex_(e);if(n){const s=n.maxKey();return s&&s.name}else return this.children_.maxKey()}getLastChild(e){const n=this.getLastChildName(e);return n?new Y(n,this.children_.get(n)):null}forEachChild(e,n){const s=this.resolveIndex_(e);return s?s.inorderTraversal(i=>n(i.name,i.node)):this.children_.inorderTraversal(n)}getIterator(e){return this.getIteratorFrom(e.minPost(),e)}getIteratorFrom(e,n){const s=this.resolveIndex_(n);if(s)return s.getIteratorFrom(e,i=>i);{const i=this.children_.getIteratorFrom(e.name,Y.Wrap);let r=i.peek();for(;r!=null&&n.compare(r,e)<0;)i.getNext(),r=i.peek();return i}}getReverseIterator(e){return this.getReverseIteratorFrom(e.maxPost(),e)}getReverseIteratorFrom(e,n){const s=this.resolveIndex_(n);if(s)return s.getReverseIteratorFrom(e,i=>i);{const i=this.children_.getReverseIteratorFrom(e.name,Y.Wrap);let r=i.peek();for(;r!=null&&n.compare(r,e)>0;)i.getNext(),r=i.peek();return i}}compareTo(e){return this.isEmpty()?e.isEmpty()?0:-1:e.isLeafNode()||e.isEmpty()?1:e===Ri?-1:0}withIndex(e){if(e===ls||this.indexMap_.hasIndex(e))return this;{const n=this.indexMap_.addIndex(e,this.children_);return new W(this.children_,this.priorityNode_,n)}}isIndexed(e){return e===ls||this.indexMap_.hasIndex(e)}equals(e){if(e===this)return!0;if(e.isLeafNode())return!1;{const n=e;if(this.getPriority().equals(n.getPriority()))if(this.children_.count()===n.children_.count()){const s=this.getIterator(me),i=n.getIterator(me);let r=s.getNext(),o=i.getNext();for(;r&&o;){if(r.name!==o.name||!r.node.equals(o.node))return!1;r=s.getNext(),o=i.getNext()}return r===null&&o===null}else return!1;else return!1}}resolveIndex_(e){return e===ls?null:this.indexMap_.get(e.toString())}}W.INTEGER_REGEXP_=/^(0|[1-9]\d*)$/;class hb extends W{constructor(){super(new $e(ol),W.EMPTY_NODE,Dt.Default)}compareTo(e){return e===this?0:1}equals(e){return e===this}getPriority(){return this}getImmediateChild(e){return W.EMPTY_NODE}isEmpty(){return!1}}const Ri=new hb;Object.defineProperties(Y,{MIN:{value:new Y(ps,W.EMPTY_NODE)},MAX:{value:new Y(Wn,Ri)}});$f.__EMPTY_NODE=W.EMPTY_NODE;Ce.__childrenNodeConstructor=W;rb(Ri);ab(Ri);/**
 * @license
 * Copyright 2017 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */const db=!0;function Te(t,e=null){if(t===null)return W.EMPTY_NODE;if(typeof t=="object"&&".priority"in t&&(e=t[".priority"]),C(e===null||typeof e=="string"||typeof e=="number"||typeof e=="object"&&".sv"in e,"Invalid priority type found: "+typeof e),typeof t=="object"&&".value"in t&&t[".value"]!==null&&(t=t[".value"]),typeof t!="object"||".sv"in t){const n=t;return new Ce(n,Te(e))}if(!(t instanceof Array)&&db){const n=[];let s=!1;if(Ge(t,(o,a)=>{if(o.substring(0,1)!=="."){const l=Te(a);l.isEmpty()||(s=s||!l.getPriority().isEmpty(),n.push(new Y(o,l)))}}),n.length===0)return W.EMPTY_NODE;const r=mr(n,ib,o=>o.name,ol);if(s){const o=mr(n,me.getCompare());return new W(r,Te(e),new Dt({".priority":o},{".priority":me}))}else return new W(r,Te(e),Dt.Default)}else{let n=W.EMPTY_NODE;return Ge(t,(s,i)=>{if(Vt(t,s)&&s.substring(0,1)!=="."){const r=Te(i);(r.isLeafNode()||!r.isEmpty())&&(n=n.updateImmediateChild(s,r))}}),n.updatePriority(Te(e))}}ob(Te);/**
 * @license
 * Copyright 2017 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */class fb extends Qr{constructor(e){super(),this.indexPath_=e,C(!Q(e)&&z(e)!==".priority","Can't create PathIndex with empty path or .priority key")}extractChild(e){return e.getChild(this.indexPath_)}isDefinedOn(e){return!e.getChild(this.indexPath_).isEmpty()}compare(e,n){const s=this.extractChild(e.node),i=this.extractChild(n.node),r=s.compareTo(i);return r===0?ws(e.name,n.name):r}makePost(e,n){const s=Te(e),i=W.EMPTY_NODE.updateChild(this.indexPath_,s);return new Y(n,i)}maxPost(){const e=W.EMPTY_NODE.updateChild(this.indexPath_,Ri);return new Y(Wn,e)}toString(){return Hf(this.indexPath_,0).join("/")}}/**
 * @license
 * Copyright 2017 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */class pb extends Qr{compare(e,n){const s=e.node.compareTo(n.node);return s===0?ws(e.name,n.name):s}isDefinedOn(e){return!0}indexedValueChanged(e,n){return!e.equals(n)}minPost(){return Y.MIN}maxPost(){return Y.MAX}makePost(e,n){const s=Te(e);return new Y(n,s)}toString(){return".value"}}const _b=new pb;/**
 * @license
 * Copyright 2017 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */function zf(t){return{type:"value",snapshotNode:t}}function _s(t,e){return{type:"child_added",snapshotNode:e,childName:t}}function li(t,e){return{type:"child_removed",snapshotNode:e,childName:t}}function ci(t,e,n){return{type:"child_changed",snapshotNode:e,childName:t,oldSnap:n}}function gb(t,e){return{type:"child_moved",snapshotNode:e,childName:t}}/**
 * @license
 * Copyright 2017 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */class al{constructor(e){this.index_=e}updateChild(e,n,s,i,r,o){C(e.isIndexed(this.index_),"A node must be indexed if only a child is updated");const a=e.getImmediateChild(n);return a.getChild(i).equals(s.getChild(i))&&a.isEmpty()===s.isEmpty()||(o!=null&&(s.isEmpty()?e.hasChild(n)?o.trackChildChange(li(n,a)):C(e.isLeafNode(),"A child remove without an old child only makes sense on a leaf node"):a.isEmpty()?o.trackChildChange(_s(n,s)):o.trackChildChange(ci(n,s,a))),e.isLeafNode()&&s.isEmpty())?e:e.updateImmediateChild(n,s).withIndex(this.index_)}updateFullNode(e,n,s){return s!=null&&(e.isLeafNode()||e.forEachChild(me,(i,r)=>{n.hasChild(i)||s.trackChildChange(li(i,r))}),n.isLeafNode()||n.forEachChild(me,(i,r)=>{if(e.hasChild(i)){const o=e.getImmediateChild(i);o.equals(r)||s.trackChildChange(ci(i,r,o))}else s.trackChildChange(_s(i,r))})),n.withIndex(this.index_)}updatePriority(e,n){return e.isEmpty()?W.EMPTY_NODE:e.updatePriority(n)}filtersNodes(){return!1}getIndexedFilter(){return this}getIndex(){return this.index_}}/**
 * @license
 * Copyright 2017 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */class ui{constructor(e){this.indexedFilter_=new al(e.getIndex()),this.index_=e.getIndex(),this.startPost_=ui.getStartPost_(e),this.endPost_=ui.getEndPost_(e),this.startIsInclusive_=!e.startAfterSet_,this.endIsInclusive_=!e.endBeforeSet_}getStartPost(){return this.startPost_}getEndPost(){return this.endPost_}matches(e){const n=this.startIsInclusive_?this.index_.compare(this.getStartPost(),e)<=0:this.index_.compare(this.getStartPost(),e)<0,s=this.endIsInclusive_?this.index_.compare(e,this.getEndPost())<=0:this.index_.compare(e,this.getEndPost())<0;return n&&s}updateChild(e,n,s,i,r,o){return this.matches(new Y(n,s))||(s=W.EMPTY_NODE),this.indexedFilter_.updateChild(e,n,s,i,r,o)}updateFullNode(e,n,s){n.isLeafNode()&&(n=W.EMPTY_NODE);let i=n.withIndex(this.index_);i=i.updatePriority(W.EMPTY_NODE);const r=this;return n.forEachChild(me,(o,a)=>{r.matches(new Y(o,a))||(i=i.updateImmediateChild(o,W.EMPTY_NODE))}),this.indexedFilter_.updateFullNode(e,i,s)}updatePriority(e,n){return e}filtersNodes(){return!0}getIndexedFilter(){return this.indexedFilter_}getIndex(){return this.index_}static getStartPost_(e){if(e.hasStart()){const n=e.getIndexStartName();return e.getIndex().makePost(e.getIndexStartValue(),n)}else return e.getIndex().minPost()}static getEndPost_(e){if(e.hasEnd()){const n=e.getIndexEndName();return e.getIndex().makePost(e.getIndexEndValue(),n)}else return e.getIndex().maxPost()}}/**
 * @license
 * Copyright 2017 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */class mb{constructor(e){this.withinDirectionalStart=n=>this.reverse_?this.withinEndPost(n):this.withinStartPost(n),this.withinDirectionalEnd=n=>this.reverse_?this.withinStartPost(n):this.withinEndPost(n),this.withinStartPost=n=>{const s=this.index_.compare(this.rangedFilter_.getStartPost(),n);return this.startIsInclusive_?s<=0:s<0},this.withinEndPost=n=>{const s=this.index_.compare(n,this.rangedFilter_.getEndPost());return this.endIsInclusive_?s<=0:s<0},this.rangedFilter_=new ui(e),this.index_=e.getIndex(),this.limit_=e.getLimit(),this.reverse_=!e.isViewFromLeft(),this.startIsInclusive_=!e.startAfterSet_,this.endIsInclusive_=!e.endBeforeSet_}updateChild(e,n,s,i,r,o){return this.rangedFilter_.matches(new Y(n,s))||(s=W.EMPTY_NODE),e.getImmediateChild(n).equals(s)?e:e.numChildren()<this.limit_?this.rangedFilter_.getIndexedFilter().updateChild(e,n,s,i,r,o):this.fullLimitUpdateChild_(e,n,s,r,o)}updateFullNode(e,n,s){let i;if(n.isLeafNode()||n.isEmpty())i=W.EMPTY_NODE.withIndex(this.index_);else if(this.limit_*2<n.numChildren()&&n.isIndexed(this.index_)){i=W.EMPTY_NODE.withIndex(this.index_);let r;this.reverse_?r=n.getReverseIteratorFrom(this.rangedFilter_.getEndPost(),this.index_):r=n.getIteratorFrom(this.rangedFilter_.getStartPost(),this.index_);let o=0;for(;r.hasNext()&&o<this.limit_;){const a=r.getNext();if(this.withinDirectionalStart(a))if(this.withinDirectionalEnd(a))i=i.updateImmediateChild(a.name,a.node),o++;else break;else continue}}else{i=n.withIndex(this.index_),i=i.updatePriority(W.EMPTY_NODE);let r;this.reverse_?r=i.getReverseIterator(this.index_):r=i.getIterator(this.index_);let o=0;for(;r.hasNext();){const a=r.getNext();o<this.limit_&&this.withinDirectionalStart(a)&&this.withinDirectionalEnd(a)?o++:i=i.updateImmediateChild(a.name,W.EMPTY_NODE)}}return this.rangedFilter_.getIndexedFilter().updateFullNode(e,i,s)}updatePriority(e,n){return e}filtersNodes(){return!0}getIndexedFilter(){return this.rangedFilter_.getIndexedFilter()}getIndex(){return this.index_}fullLimitUpdateChild_(e,n,s,i,r){let o;if(this.reverse_){const u=this.index_.getCompare();o=(d,_)=>u(_,d)}else o=this.index_.getCompare();const a=e;C(a.numChildren()===this.limit_,"");const l=new Y(n,s),c=this.reverse_?a.getFirstChild(this.index_):a.getLastChild(this.index_),h=this.rangedFilter_.matches(l);if(a.hasChild(n)){const u=a.getImmediateChild(n);let d=i.getChildAfterChild(this.index_,c,this.reverse_);for(;d!=null&&(d.name===n||a.hasChild(d.name));)d=i.getChildAfterChild(this.index_,d,this.reverse_);const _=d==null?1:o(d,l);if(h&&!s.isEmpty()&&_>=0)return r!=null&&r.trackChildChange(ci(n,s,u)),a.updateImmediateChild(n,s);{r!=null&&r.trackChildChange(li(n,u));const I=a.updateImmediateChild(n,W.EMPTY_NODE);return d!=null&&this.rangedFilter_.matches(d)?(r!=null&&r.trackChildChange(_s(d.name,d.node)),I.updateImmediateChild(d.name,d.node)):I}}else return s.isEmpty()?e:h&&o(c,l)>=0?(r!=null&&(r.trackChildChange(li(c.name,c.node)),r.trackChildChange(_s(n,s))),a.updateImmediateChild(n,s).updateImmediateChild(c.name,W.EMPTY_NODE)):e}}/**
 * @license
 * Copyright 2017 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */class ll{constructor(){this.limitSet_=!1,this.startSet_=!1,this.startNameSet_=!1,this.startAfterSet_=!1,this.endSet_=!1,this.endNameSet_=!1,this.endBeforeSet_=!1,this.limit_=0,this.viewFrom_="",this.indexStartValue_=null,this.indexStartName_="",this.indexEndValue_=null,this.indexEndName_="",this.index_=me}hasStart(){return this.startSet_}isViewFromLeft(){return this.viewFrom_===""?this.startSet_:this.viewFrom_==="l"}getIndexStartValue(){return C(this.startSet_,"Only valid if start has been set"),this.indexStartValue_}getIndexStartName(){return C(this.startSet_,"Only valid if start has been set"),this.startNameSet_?this.indexStartName_:ps}hasEnd(){return this.endSet_}getIndexEndValue(){return C(this.endSet_,"Only valid if end has been set"),this.indexEndValue_}getIndexEndName(){return C(this.endSet_,"Only valid if end has been set"),this.endNameSet_?this.indexEndName_:Wn}hasLimit(){return this.limitSet_}hasAnchoredLimit(){return this.limitSet_&&this.viewFrom_!==""}getLimit(){return C(this.limitSet_,"Only valid if limit has been set"),this.limit_}getIndex(){return this.index_}loadsAllData(){return!(this.startSet_||this.endSet_||this.limitSet_)}isDefault(){return this.loadsAllData()&&this.index_===me}copy(){const e=new ll;return e.limitSet_=this.limitSet_,e.limit_=this.limit_,e.startSet_=this.startSet_,e.startAfterSet_=this.startAfterSet_,e.indexStartValue_=this.indexStartValue_,e.startNameSet_=this.startNameSet_,e.indexStartName_=this.indexStartName_,e.endSet_=this.endSet_,e.endBeforeSet_=this.endBeforeSet_,e.indexEndValue_=this.indexEndValue_,e.endNameSet_=this.endNameSet_,e.indexEndName_=this.indexEndName_,e.index_=this.index_,e.viewFrom_=this.viewFrom_,e}}function vb(t){return t.loadsAllData()?new al(t.getIndex()):t.hasLimit()?new mb(t):new ui(t)}function Su(t){const e={};if(t.isDefault())return e;let n;if(t.index_===me?n="$priority":t.index_===_b?n="$value":t.index_===ls?n="$key":(C(t.index_ instanceof fb,"Unrecognized index type!"),n=t.index_.toString()),e.orderBy=we(n),t.startSet_){const s=t.startAfterSet_?"startAfter":"startAt";e[s]=we(t.indexStartValue_),t.startNameSet_&&(e[s]+=","+we(t.indexStartName_))}if(t.endSet_){const s=t.endBeforeSet_?"endBefore":"endAt";e[s]=we(t.indexEndValue_),t.endNameSet_&&(e[s]+=","+we(t.indexEndName_))}return t.limitSet_&&(t.isViewFromLeft()?e.limitToFirst=t.limit_:e.limitToLast=t.limit_),e}function Tu(t){const e={};if(t.startSet_&&(e.sp=t.indexStartValue_,t.startNameSet_&&(e.sn=t.indexStartName_),e.sin=!t.startAfterSet_),t.endSet_&&(e.ep=t.indexEndValue_,t.endNameSet_&&(e.en=t.indexEndName_),e.ein=!t.endBeforeSet_),t.limitSet_){e.l=t.limit_;let n=t.viewFrom_;n===""&&(t.isViewFromLeft()?n="l":n="r"),e.vf=n}return t.index_!==me&&(e.i=t.index_.toString()),e}/**
 * @license
 * Copyright 2017 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */class vr extends Ff{constructor(e,n,s,i){super(),this.repoInfo_=e,this.onDataUpdate_=n,this.authTokenProvider_=s,this.appCheckTokenProvider_=i,this.log_=Ti("p:rest:"),this.listens_={}}reportStats(e){throw new Error("Method not implemented.")}static getListenId_(e,n){return n!==void 0?"tag$"+n:(C(e._queryParams.isDefault(),"should have a tag if it's not a default query."),e._path.toString())}listen(e,n,s,i){const r=e._path.toString();this.log_("Listen called for "+r+" "+e._queryIdentifier);const o=vr.getListenId_(e,s),a={};this.listens_[o]=a;const l=Su(e._queryParams);this.restRequest_(r+".json",l,(c,h)=>{let u=h;if(c===404&&(u=null,c=null),c===null&&this.onDataUpdate_(r,u,!1,s),ds(this.listens_,o)===a){let d;c?c===401?d="permission_denied":d="rest_error:"+c:d="ok",i(d,null)}})}unlisten(e,n){const s=vr.getListenId_(e,n);delete this.listens_[s]}get(e){const n=Su(e._queryParams),s=e._path.toString(),i=new Gr;return this.restRequest_(s+".json",n,(r,o)=>{let a=o;r===404&&(a=null,r=null),r===null?(this.onDataUpdate_(s,a,!1,null),i.resolve(a)):i.reject(new Error(a))}),i.promise}refreshAuthToken(e){}restRequest_(e,n={},s){return n.format="export",Promise.all([this.authTokenProvider_.getToken(!1),this.appCheckTokenProvider_.getToken(!1)]).then(([i,r])=>{i&&i.accessToken&&(n.auth=i.accessToken),r&&r.token&&(n.ac=r.token);const o=(this.repoInfo_.secure?"https://":"http://")+this.repoInfo_.host+e+"?ns="+this.repoInfo_.namespace+vs(n);this.log_("Sending REST request for "+o);const a=new XMLHttpRequest;a.onreadystatechange=()=>{if(s&&a.readyState===4){this.log_("REST Response for "+o+" received. status:",a.status,"response:",a.responseText);let l=null;if(a.status>=200&&a.status<300){try{l=si(a.responseText)}catch{je("Failed to parse JSON response for "+o+": "+a.responseText)}s(null,l)}else a.status!==401&&a.status!==404&&je("Got unsuccessful REST response for "+o+" Status: "+a.status),s(a.status);s=null}},a.open("GET",o,!0),a.send()})}}/**
 * @license
 * Copyright 2017 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */class yb{constructor(){this.rootNode_=W.EMPTY_NODE}getNode(e){return this.rootNode_.getChild(e)}updateSnapshot(e,n){this.rootNode_=this.rootNode_.updateChild(e,n)}}/**
 * @license
 * Copyright 2017 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */function yr(){return{value:null,children:new Map}}function Yf(t,e,n){if(Q(e))t.value=n,t.children.clear();else if(t.value!==null)t.value=t.value.updateChild(e,n);else{const s=z(e);t.children.has(s)||t.children.set(s,yr());const i=t.children.get(s);e=he(e),Yf(i,e,n)}}function la(t,e,n){t.value!==null?n(e,t.value):Eb(t,(s,i)=>{const r=new le(e.toString()+"/"+s);la(i,r,n)})}function Eb(t,e){t.children.forEach((n,s)=>{e(s,n)})}/**
 * @license
 * Copyright 2017 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */class wb{constructor(e){this.collection_=e,this.last_=null}get(){const e=this.collection_.get(),n=Object.assign({},e);return this.last_&&Ge(this.last_,(s,i)=>{n[s]=n[s]-i}),this.last_=e,n}}/**
 * @license
 * Copyright 2017 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */const Ru=10*1e3,Ib=30*1e3,bb=5*60*1e3;class Cb{constructor(e,n){this.server_=n,this.statsToReport_={},this.statsListener_=new wb(e);const s=Ru+(Ib-Ru)*Math.random();Ks(this.reportStats_.bind(this),Math.floor(s))}reportStats_(){const e=this.statsListener_.get(),n={};let s=!1;Ge(e,(i,r)=>{r>0&&Vt(this.statsToReport_,i)&&(n[i]=r,s=!0)}),s&&this.server_.reportStats(n),Ks(this.reportStats_.bind(this),Math.floor(Math.random()*2*bb))}}/**
 * @license
 * Copyright 2017 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */var ot;(function(t){t[t.OVERWRITE=0]="OVERWRITE",t[t.MERGE=1]="MERGE",t[t.ACK_USER_WRITE=2]="ACK_USER_WRITE",t[t.LISTEN_COMPLETE=3]="LISTEN_COMPLETE"})(ot||(ot={}));function Qf(){return{fromUser:!0,fromServer:!1,queryId:null,tagged:!1}}function cl(){return{fromUser:!1,fromServer:!0,queryId:null,tagged:!1}}function ul(t){return{fromUser:!1,fromServer:!0,queryId:t,tagged:!0}}/**
 * @license
 * Copyright 2017 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */class Er{constructor(e,n,s){this.path=e,this.affectedTree=n,this.revert=s,this.type=ot.ACK_USER_WRITE,this.source=Qf()}operationForChild(e){if(Q(this.path)){if(this.affectedTree.value!=null)return C(this.affectedTree.children.isEmpty(),"affectedTree should not have overlapping affected paths."),this;{const n=this.affectedTree.subtree(new le(e));return new Er(ee(),n,this.revert)}}else return C(z(this.path)===e,"operationForChild called for unrelated child."),new Er(he(this.path),this.affectedTree,this.revert)}}/**
 * @license
 * Copyright 2017 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */class hi{constructor(e,n){this.source=e,this.path=n,this.type=ot.LISTEN_COMPLETE}operationForChild(e){return Q(this.path)?new hi(this.source,ee()):new hi(this.source,he(this.path))}}/**
 * @license
 * Copyright 2017 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */class Hn{constructor(e,n,s){this.source=e,this.path=n,this.snap=s,this.type=ot.OVERWRITE}operationForChild(e){return Q(this.path)?new Hn(this.source,ee(),this.snap.getImmediateChild(e)):new Hn(this.source,he(this.path),this.snap)}}/**
 * @license
 * Copyright 2017 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */class di{constructor(e,n,s){this.source=e,this.path=n,this.children=s,this.type=ot.MERGE}operationForChild(e){if(Q(this.path)){const n=this.children.subtree(new le(e));return n.isEmpty()?null:n.value?new Hn(this.source,ee(),n.value):new di(this.source,ee(),n)}else return C(z(this.path)===e,"Can't get a merge for a child not on the path of the operation"),new di(this.source,he(this.path),this.children)}toString(){return"Operation("+this.path+": "+this.source.toString()+" merge: "+this.children.toString()+")"}}/**
 * @license
 * Copyright 2017 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */class dn{constructor(e,n,s){this.node_=e,this.fullyInitialized_=n,this.filtered_=s}isFullyInitialized(){return this.fullyInitialized_}isFiltered(){return this.filtered_}isCompleteForPath(e){if(Q(e))return this.isFullyInitialized()&&!this.filtered_;const n=z(e);return this.isCompleteForChild(n)}isCompleteForChild(e){return this.isFullyInitialized()&&!this.filtered_||this.node_.hasChild(e)}getNode(){return this.node_}}/**
 * @license
 * Copyright 2017 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */class Sb{constructor(e){this.query_=e,this.index_=this.query_._queryParams.getIndex()}}function Tb(t,e,n,s){const i=[],r=[];return e.forEach(o=>{o.type==="child_changed"&&t.index_.indexedValueChanged(o.oldSnap,o.snapshotNode)&&r.push(gb(o.childName,o.snapshotNode))}),ks(t,i,"child_removed",e,s,n),ks(t,i,"child_added",e,s,n),ks(t,i,"child_moved",r,s,n),ks(t,i,"child_changed",e,s,n),ks(t,i,"value",e,s,n),i}function ks(t,e,n,s,i,r){const o=s.filter(a=>a.type===n);o.sort((a,l)=>Pb(t,a,l)),o.forEach(a=>{const l=Rb(t,a,r);i.forEach(c=>{c.respondsTo(a.type)&&e.push(c.createEvent(l,t.query_))})})}function Rb(t,e,n){return e.type==="value"||e.type==="child_removed"||(e.prevName=n.getPredecessorChildName(e.childName,e.snapshotNode,t.index_)),e}function Pb(t,e,n){if(e.childName==null||n.childName==null)throw ms("Should only compare child_ events.");const s=new Y(e.childName,e.snapshotNode),i=new Y(n.childName,n.snapshotNode);return t.index_.compare(s,i)}/**
 * @license
 * Copyright 2017 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */function Jr(t,e){return{eventCache:t,serverCache:e}}function qs(t,e,n,s){return Jr(new dn(e,n,s),t.serverCache)}function Jf(t,e,n,s){return Jr(t.eventCache,new dn(e,n,s))}function wr(t){return t.eventCache.isFullyInitialized()?t.eventCache.getNode():null}function Vn(t){return t.serverCache.isFullyInitialized()?t.serverCache.getNode():null}/**
 * @license
 * Copyright 2017 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */let ko;const Ab=()=>(ko||(ko=new $e(pI)),ko);class pe{constructor(e,n=Ab()){this.value=e,this.children=n}static fromObject(e){let n=new pe(null);return Ge(e,(s,i)=>{n=n.set(new le(s),i)}),n}isEmpty(){return this.value===null&&this.children.isEmpty()}findRootMostMatchingPathAndValue(e,n){if(this.value!=null&&n(this.value))return{path:ee(),value:this.value};if(Q(e))return null;{const s=z(e),i=this.children.get(s);if(i!==null){const r=i.findRootMostMatchingPathAndValue(he(e),n);return r!=null?{path:Ie(new le(s),r.path),value:r.value}:null}else return null}}findRootMostValueAndPath(e){return this.findRootMostMatchingPathAndValue(e,()=>!0)}subtree(e){if(Q(e))return this;{const n=z(e),s=this.children.get(n);return s!==null?s.subtree(he(e)):new pe(null)}}set(e,n){if(Q(e))return new pe(n,this.children);{const s=z(e),r=(this.children.get(s)||new pe(null)).set(he(e),n),o=this.children.insert(s,r);return new pe(this.value,o)}}remove(e){if(Q(e))return this.children.isEmpty()?new pe(null):new pe(null,this.children);{const n=z(e),s=this.children.get(n);if(s){const i=s.remove(he(e));let r;return i.isEmpty()?r=this.children.remove(n):r=this.children.insert(n,i),this.value===null&&r.isEmpty()?new pe(null):new pe(this.value,r)}else return this}}get(e){if(Q(e))return this.value;{const n=z(e),s=this.children.get(n);return s?s.get(he(e)):null}}setTree(e,n){if(Q(e))return n;{const s=z(e),r=(this.children.get(s)||new pe(null)).setTree(he(e),n);let o;return r.isEmpty()?o=this.children.remove(s):o=this.children.insert(s,r),new pe(this.value,o)}}fold(e){return this.fold_(ee(),e)}fold_(e,n){const s={};return this.children.inorderTraversal((i,r)=>{s[i]=r.fold_(Ie(e,i),n)}),n(e,this.value,s)}findOnPath(e,n){return this.findOnPath_(e,ee(),n)}findOnPath_(e,n,s){const i=this.value?s(n,this.value):!1;if(i)return i;if(Q(e))return null;{const r=z(e),o=this.children.get(r);return o?o.findOnPath_(he(e),Ie(n,r),s):null}}foreachOnPath(e,n){return this.foreachOnPath_(e,ee(),n)}foreachOnPath_(e,n,s){if(Q(e))return this;{this.value&&s(n,this.value);const i=z(e),r=this.children.get(i);return r?r.foreachOnPath_(he(e),Ie(n,i),s):new pe(null)}}foreach(e){this.foreach_(ee(),e)}foreach_(e,n){this.children.inorderTraversal((s,i)=>{i.foreach_(Ie(e,s),n)}),this.value&&n(e,this.value)}foreachChild(e){this.children.inorderTraversal((n,s)=>{s.value&&e(n,s.value)})}}/**
 * @license
 * Copyright 2017 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */class ct{constructor(e){this.writeTree_=e}static empty(){return new ct(new pe(null))}}function zs(t,e,n){if(Q(e))return new ct(new pe(n));{const s=t.writeTree_.findRootMostValueAndPath(e);if(s!=null){const i=s.path;let r=s.value;const o=Le(i,e);return r=r.updateChild(o,n),new ct(t.writeTree_.set(i,r))}else{const i=new pe(n),r=t.writeTree_.setTree(e,i);return new ct(r)}}}function Pu(t,e,n){let s=t;return Ge(n,(i,r)=>{s=zs(s,Ie(e,i),r)}),s}function Au(t,e){if(Q(e))return ct.empty();{const n=t.writeTree_.setTree(e,new pe(null));return new ct(n)}}function ca(t,e){return $n(t,e)!=null}function $n(t,e){const n=t.writeTree_.findRootMostValueAndPath(e);return n!=null?t.writeTree_.get(n.path).getChild(Le(n.path,e)):null}function Nu(t){const e=[],n=t.writeTree_.value;return n!=null?n.isLeafNode()||n.forEachChild(me,(s,i)=>{e.push(new Y(s,i))}):t.writeTree_.children.inorderTraversal((s,i)=>{i.value!=null&&e.push(new Y(s,i.value))}),e}function cn(t,e){if(Q(e))return t;{const n=$n(t,e);return n!=null?new ct(new pe(n)):new ct(t.writeTree_.subtree(e))}}function ua(t){return t.writeTree_.isEmpty()}function gs(t,e){return Xf(ee(),t.writeTree_,e)}function Xf(t,e,n){if(e.value!=null)return n.updateChild(t,e.value);{let s=null;return e.children.inorderTraversal((i,r)=>{i===".priority"?(C(r.value!==null,"Priority writes must always be leaf nodes"),s=r.value):n=Xf(Ie(t,i),r,n)}),!n.getChild(t).isEmpty()&&s!==null&&(n=n.updateChild(Ie(t,".priority"),s)),n}}/**
 * @license
 * Copyright 2017 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */function Xr(t,e){return np(e,t)}function Nb(t,e,n,s,i){C(s>t.lastWriteId,"Stacking an older write on top of newer ones"),i===void 0&&(i=!0),t.allWrites.push({path:e,snap:n,writeId:s,visible:i}),i&&(t.visibleWrites=zs(t.visibleWrites,e,n)),t.lastWriteId=s}function kb(t,e){for(let n=0;n<t.allWrites.length;n++){const s=t.allWrites[n];if(s.writeId===e)return s}return null}function Ob(t,e){const n=t.allWrites.findIndex(a=>a.writeId===e);C(n>=0,"removeWrite called with nonexistent writeId.");const s=t.allWrites[n];t.allWrites.splice(n,1);let i=s.visible,r=!1,o=t.allWrites.length-1;for(;i&&o>=0;){const a=t.allWrites[o];a.visible&&(o>=n&&xb(a,s.path)?i=!1:rt(s.path,a.path)&&(r=!0)),o--}if(i){if(r)return Db(t),!0;if(s.snap)t.visibleWrites=Au(t.visibleWrites,s.path);else{const a=s.children;Ge(a,l=>{t.visibleWrites=Au(t.visibleWrites,Ie(s.path,l))})}return!0}else return!1}function xb(t,e){if(t.snap)return rt(t.path,e);for(const n in t.children)if(t.children.hasOwnProperty(n)&&rt(Ie(t.path,n),e))return!0;return!1}function Db(t){t.visibleWrites=Zf(t.allWrites,Mb,ee()),t.allWrites.length>0?t.lastWriteId=t.allWrites[t.allWrites.length-1].writeId:t.lastWriteId=-1}function Mb(t){return t.visible}function Zf(t,e,n){let s=ct.empty();for(let i=0;i<t.length;++i){const r=t[i];if(e(r)){const o=r.path;let a;if(r.snap)rt(n,o)?(a=Le(n,o),s=zs(s,a,r.snap)):rt(o,n)&&(a=Le(o,n),s=zs(s,ee(),r.snap.getChild(a)));else if(r.children){if(rt(n,o))a=Le(n,o),s=Pu(s,a,r.children);else if(rt(o,n))if(a=Le(o,n),Q(a))s=Pu(s,ee(),r.children);else{const l=ds(r.children,z(a));if(l){const c=l.getChild(he(a));s=zs(s,ee(),c)}}}else throw ms("WriteRecord should have .snap or .children")}}return s}function ep(t,e,n,s,i){if(!s&&!i){const r=$n(t.visibleWrites,e);if(r!=null)return r;{const o=cn(t.visibleWrites,e);if(ua(o))return n;if(n==null&&!ca(o,ee()))return null;{const a=n||W.EMPTY_NODE;return gs(o,a)}}}else{const r=cn(t.visibleWrites,e);if(!i&&ua(r))return n;if(!i&&n==null&&!ca(r,ee()))return null;{const o=function(c){return(c.visible||i)&&(!s||!~s.indexOf(c.writeId))&&(rt(c.path,e)||rt(e,c.path))},a=Zf(t.allWrites,o,e),l=n||W.EMPTY_NODE;return gs(a,l)}}}function Lb(t,e,n){let s=W.EMPTY_NODE;const i=$n(t.visibleWrites,e);if(i)return i.isLeafNode()||i.forEachChild(me,(r,o)=>{s=s.updateImmediateChild(r,o)}),s;if(n){const r=cn(t.visibleWrites,e);return n.forEachChild(me,(o,a)=>{const l=gs(cn(r,new le(o)),a);s=s.updateImmediateChild(o,l)}),Nu(r).forEach(o=>{s=s.updateImmediateChild(o.name,o.node)}),s}else{const r=cn(t.visibleWrites,e);return Nu(r).forEach(o=>{s=s.updateImmediateChild(o.name,o.node)}),s}}function Fb(t,e,n,s,i){C(s||i,"Either existingEventSnap or existingServerSnap must exist");const r=Ie(e,n);if(ca(t.visibleWrites,r))return null;{const o=cn(t.visibleWrites,r);return ua(o)?i.getChild(n):gs(o,i.getChild(n))}}function Ub(t,e,n,s){const i=Ie(e,n),r=$n(t.visibleWrites,i);if(r!=null)return r;if(s.isCompleteForChild(n)){const o=cn(t.visibleWrites,i);return gs(o,s.getNode().getImmediateChild(n))}else return null}function Wb(t,e){return $n(t.visibleWrites,e)}function Hb(t,e,n,s,i,r,o){let a;const l=cn(t.visibleWrites,e),c=$n(l,ee());if(c!=null)a=c;else if(n!=null)a=gs(l,n);else return[];if(a=a.withIndex(o),!a.isEmpty()&&!a.isLeafNode()){const h=[],u=o.getCompare(),d=r?a.getReverseIteratorFrom(s,o):a.getIteratorFrom(s,o);let _=d.getNext();for(;_&&h.length<i;)u(_,s)!==0&&h.push(_),_=d.getNext();return h}else return[]}function Vb(){return{visibleWrites:ct.empty(),allWrites:[],lastWriteId:-1}}function Ir(t,e,n,s){return ep(t.writeTree,t.treePath,e,n,s)}function hl(t,e){return Lb(t.writeTree,t.treePath,e)}function ku(t,e,n,s){return Fb(t.writeTree,t.treePath,e,n,s)}function br(t,e){return Wb(t.writeTree,Ie(t.treePath,e))}function Bb(t,e,n,s,i,r){return Hb(t.writeTree,t.treePath,e,n,s,i,r)}function dl(t,e,n){return Ub(t.writeTree,t.treePath,e,n)}function tp(t,e){return np(Ie(t.treePath,e),t.writeTree)}function np(t,e){return{treePath:t,writeTree:e}}/**
 * @license
 * Copyright 2017 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */class $b{constructor(){this.changeMap=new Map}trackChildChange(e){const n=e.type,s=e.childName;C(n==="child_added"||n==="child_changed"||n==="child_removed","Only child changes supported for tracking"),C(s!==".priority","Only non-priority child changes can be tracked.");const i=this.changeMap.get(s);if(i){const r=i.type;if(n==="child_added"&&r==="child_removed")this.changeMap.set(s,ci(s,e.snapshotNode,i.snapshotNode));else if(n==="child_removed"&&r==="child_added")this.changeMap.delete(s);else if(n==="child_removed"&&r==="child_changed")this.changeMap.set(s,li(s,i.oldSnap));else if(n==="child_changed"&&r==="child_added")this.changeMap.set(s,_s(s,e.snapshotNode));else if(n==="child_changed"&&r==="child_changed")this.changeMap.set(s,ci(s,e.snapshotNode,i.oldSnap));else throw ms("Illegal combination of changes: "+e+" occurred after "+i)}else this.changeMap.set(s,e)}getChanges(){return Array.from(this.changeMap.values())}}/**
 * @license
 * Copyright 2017 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */class jb{getCompleteChild(e){return null}getChildAfterChild(e,n,s){return null}}const sp=new jb;class fl{constructor(e,n,s=null){this.writes_=e,this.viewCache_=n,this.optCompleteServerCache_=s}getCompleteChild(e){const n=this.viewCache_.eventCache;if(n.isCompleteForChild(e))return n.getNode().getImmediateChild(e);{const s=this.optCompleteServerCache_!=null?new dn(this.optCompleteServerCache_,!0,!1):this.viewCache_.serverCache;return dl(this.writes_,e,s)}}getChildAfterChild(e,n,s){const i=this.optCompleteServerCache_!=null?this.optCompleteServerCache_:Vn(this.viewCache_),r=Bb(this.writes_,i,n,1,s,e);return r.length===0?null:r[0]}}/**
 * @license
 * Copyright 2017 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */function Gb(t){return{filter:t}}function Kb(t,e){C(e.eventCache.getNode().isIndexed(t.filter.getIndex()),"Event snap not indexed"),C(e.serverCache.getNode().isIndexed(t.filter.getIndex()),"Server snap not indexed")}function qb(t,e,n,s,i){const r=new $b;let o,a;if(n.type===ot.OVERWRITE){const c=n;c.source.fromUser?o=ha(t,e,c.path,c.snap,s,i,r):(C(c.source.fromServer,"Unknown source."),a=c.source.tagged||e.serverCache.isFiltered()&&!Q(c.path),o=Cr(t,e,c.path,c.snap,s,i,a,r))}else if(n.type===ot.MERGE){const c=n;c.source.fromUser?o=Yb(t,e,c.path,c.children,s,i,r):(C(c.source.fromServer,"Unknown source."),a=c.source.tagged||e.serverCache.isFiltered(),o=da(t,e,c.path,c.children,s,i,a,r))}else if(n.type===ot.ACK_USER_WRITE){const c=n;c.revert?o=Xb(t,e,c.path,s,i,r):o=Qb(t,e,c.path,c.affectedTree,s,i,r)}else if(n.type===ot.LISTEN_COMPLETE)o=Jb(t,e,n.path,s,r);else throw ms("Unknown operation type: "+n.type);const l=r.getChanges();return zb(e,o,l),{viewCache:o,changes:l}}function zb(t,e,n){const s=e.eventCache;if(s.isFullyInitialized()){const i=s.getNode().isLeafNode()||s.getNode().isEmpty(),r=wr(t);(n.length>0||!t.eventCache.isFullyInitialized()||i&&!s.getNode().equals(r)||!s.getNode().getPriority().equals(r.getPriority()))&&n.push(zf(wr(e)))}}function ip(t,e,n,s,i,r){const o=e.eventCache;if(br(s,n)!=null)return e;{let a,l;if(Q(n))if(C(e.serverCache.isFullyInitialized(),"If change path is empty, we must have complete server data"),e.serverCache.isFiltered()){const c=Vn(e),h=c instanceof W?c:W.EMPTY_NODE,u=hl(s,h);a=t.filter.updateFullNode(e.eventCache.getNode(),u,r)}else{const c=Ir(s,Vn(e));a=t.filter.updateFullNode(e.eventCache.getNode(),c,r)}else{const c=z(n);if(c===".priority"){C(hn(n)===1,"Can't have a priority with additional path components");const h=o.getNode();l=e.serverCache.getNode();const u=ku(s,n,h,l);u!=null?a=t.filter.updatePriority(h,u):a=o.getNode()}else{const h=he(n);let u;if(o.isCompleteForChild(c)){l=e.serverCache.getNode();const d=ku(s,n,o.getNode(),l);d!=null?u=o.getNode().getImmediateChild(c).updateChild(h,d):u=o.getNode().getImmediateChild(c)}else u=dl(s,c,e.serverCache);u!=null?a=t.filter.updateChild(o.getNode(),c,u,h,i,r):a=o.getNode()}}return qs(e,a,o.isFullyInitialized()||Q(n),t.filter.filtersNodes())}}function Cr(t,e,n,s,i,r,o,a){const l=e.serverCache;let c;const h=o?t.filter:t.filter.getIndexedFilter();if(Q(n))c=h.updateFullNode(l.getNode(),s,null);else if(h.filtersNodes()&&!l.isFiltered()){const _=l.getNode().updateChild(n,s);c=h.updateFullNode(l.getNode(),_,null)}else{const _=z(n);if(!l.isCompleteForPath(n)&&hn(n)>1)return e;const m=he(n),O=l.getNode().getImmediateChild(_).updateChild(m,s);_===".priority"?c=h.updatePriority(l.getNode(),O):c=h.updateChild(l.getNode(),_,O,m,sp,null)}const u=Jf(e,c,l.isFullyInitialized()||Q(n),h.filtersNodes()),d=new fl(i,u,r);return ip(t,u,n,i,d,a)}function ha(t,e,n,s,i,r,o){const a=e.eventCache;let l,c;const h=new fl(i,e,r);if(Q(n))c=t.filter.updateFullNode(e.eventCache.getNode(),s,o),l=qs(e,c,!0,t.filter.filtersNodes());else{const u=z(n);if(u===".priority")c=t.filter.updatePriority(e.eventCache.getNode(),s),l=qs(e,c,a.isFullyInitialized(),a.isFiltered());else{const d=he(n),_=a.getNode().getImmediateChild(u);let m;if(Q(d))m=s;else{const I=h.getCompleteChild(u);I!=null?Wf(d)===".priority"&&I.getChild(Vf(d)).isEmpty()?m=I:m=I.updateChild(d,s):m=W.EMPTY_NODE}if(_.equals(m))l=e;else{const I=t.filter.updateChild(a.getNode(),u,m,d,h,o);l=qs(e,I,a.isFullyInitialized(),t.filter.filtersNodes())}}}return l}function Ou(t,e){return t.eventCache.isCompleteForChild(e)}function Yb(t,e,n,s,i,r,o){let a=e;return s.foreach((l,c)=>{const h=Ie(n,l);Ou(e,z(h))&&(a=ha(t,a,h,c,i,r,o))}),s.foreach((l,c)=>{const h=Ie(n,l);Ou(e,z(h))||(a=ha(t,a,h,c,i,r,o))}),a}function xu(t,e,n){return n.foreach((s,i)=>{e=e.updateChild(s,i)}),e}function da(t,e,n,s,i,r,o,a){if(e.serverCache.getNode().isEmpty()&&!e.serverCache.isFullyInitialized())return e;let l=e,c;Q(n)?c=s:c=new pe(null).setTree(n,s);const h=e.serverCache.getNode();return c.children.inorderTraversal((u,d)=>{if(h.hasChild(u)){const _=e.serverCache.getNode().getImmediateChild(u),m=xu(t,_,d);l=Cr(t,l,new le(u),m,i,r,o,a)}}),c.children.inorderTraversal((u,d)=>{const _=!e.serverCache.isCompleteForChild(u)&&d.value===null;if(!h.hasChild(u)&&!_){const m=e.serverCache.getNode().getImmediateChild(u),I=xu(t,m,d);l=Cr(t,l,new le(u),I,i,r,o,a)}}),l}function Qb(t,e,n,s,i,r,o){if(br(i,n)!=null)return e;const a=e.serverCache.isFiltered(),l=e.serverCache;if(s.value!=null){if(Q(n)&&l.isFullyInitialized()||l.isCompleteForPath(n))return Cr(t,e,n,l.getNode().getChild(n),i,r,a,o);if(Q(n)){let c=new pe(null);return l.getNode().forEachChild(ls,(h,u)=>{c=c.set(new le(h),u)}),da(t,e,n,c,i,r,a,o)}else return e}else{let c=new pe(null);return s.foreach((h,u)=>{const d=Ie(n,h);l.isCompleteForPath(d)&&(c=c.set(h,l.getNode().getChild(d)))}),da(t,e,n,c,i,r,a,o)}}function Jb(t,e,n,s,i){const r=e.serverCache,o=Jf(e,r.getNode(),r.isFullyInitialized()||Q(n),r.isFiltered());return ip(t,o,n,s,sp,i)}function Xb(t,e,n,s,i,r){let o;if(br(s,n)!=null)return e;{const a=new fl(s,e,i),l=e.eventCache.getNode();let c;if(Q(n)||z(n)===".priority"){let h;if(e.serverCache.isFullyInitialized())h=Ir(s,Vn(e));else{const u=e.serverCache.getNode();C(u instanceof W,"serverChildren would be complete if leaf node"),h=hl(s,u)}h=h,c=t.filter.updateFullNode(l,h,r)}else{const h=z(n);let u=dl(s,h,e.serverCache);u==null&&e.serverCache.isCompleteForChild(h)&&(u=l.getImmediateChild(h)),u!=null?c=t.filter.updateChild(l,h,u,he(n),a,r):e.eventCache.getNode().hasChild(h)?c=t.filter.updateChild(l,h,W.EMPTY_NODE,he(n),a,r):c=l,c.isEmpty()&&e.serverCache.isFullyInitialized()&&(o=Ir(s,Vn(e)),o.isLeafNode()&&(c=t.filter.updateFullNode(c,o,r)))}return o=e.serverCache.isFullyInitialized()||br(s,ee())!=null,qs(e,c,o,t.filter.filtersNodes())}}/**
 * @license
 * Copyright 2017 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */class Zb{constructor(e,n){this.query_=e,this.eventRegistrations_=[];const s=this.query_._queryParams,i=new al(s.getIndex()),r=vb(s);this.processor_=Gb(r);const o=n.serverCache,a=n.eventCache,l=i.updateFullNode(W.EMPTY_NODE,o.getNode(),null),c=r.updateFullNode(W.EMPTY_NODE,a.getNode(),null),h=new dn(l,o.isFullyInitialized(),i.filtersNodes()),u=new dn(c,a.isFullyInitialized(),r.filtersNodes());this.viewCache_=Jr(u,h),this.eventGenerator_=new Sb(this.query_)}get query(){return this.query_}}function eC(t){return t.viewCache_.serverCache.getNode()}function tC(t){return wr(t.viewCache_)}function nC(t,e){const n=Vn(t.viewCache_);return n&&(t.query._queryParams.loadsAllData()||!Q(e)&&!n.getImmediateChild(z(e)).isEmpty())?n.getChild(e):null}function Du(t){return t.eventRegistrations_.length===0}function sC(t,e){t.eventRegistrations_.push(e)}function Mu(t,e,n){const s=[];if(n){C(e==null,"A cancel should cancel all event registrations.");const i=t.query._path;t.eventRegistrations_.forEach(r=>{const o=r.createCancelEvent(n,i);o&&s.push(o)})}if(e){let i=[];for(let r=0;r<t.eventRegistrations_.length;++r){const o=t.eventRegistrations_[r];if(!o.matches(e))i.push(o);else if(e.hasAnyCallback()){i=i.concat(t.eventRegistrations_.slice(r+1));break}}t.eventRegistrations_=i}else t.eventRegistrations_=[];return s}function Lu(t,e,n,s){e.type===ot.MERGE&&e.source.queryId!==null&&(C(Vn(t.viewCache_),"We should always have a full cache before handling merges"),C(wr(t.viewCache_),"Missing event cache, even though we have a server cache"));const i=t.viewCache_,r=qb(t.processor_,i,e,n,s);return Kb(t.processor_,r.viewCache),C(r.viewCache.serverCache.isFullyInitialized()||!i.serverCache.isFullyInitialized(),"Once a server snap is complete, it should never go back"),t.viewCache_=r.viewCache,rp(t,r.changes,r.viewCache.eventCache.getNode(),null)}function iC(t,e){const n=t.viewCache_.eventCache,s=[];return n.getNode().isLeafNode()||n.getNode().forEachChild(me,(r,o)=>{s.push(_s(r,o))}),n.isFullyInitialized()&&s.push(zf(n.getNode())),rp(t,s,n.getNode(),e)}function rp(t,e,n,s){const i=s?[s]:t.eventRegistrations_;return Tb(t.eventGenerator_,e,n,i)}/**
 * @license
 * Copyright 2017 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */let Sr;class op{constructor(){this.views=new Map}}function rC(t){C(!Sr,"__referenceConstructor has already been defined"),Sr=t}function oC(){return C(Sr,"Reference.ts has not been loaded"),Sr}function aC(t){return t.views.size===0}function pl(t,e,n,s){const i=e.source.queryId;if(i!==null){const r=t.views.get(i);return C(r!=null,"SyncTree gave us an op for an invalid query."),Lu(r,e,n,s)}else{let r=[];for(const o of t.views.values())r=r.concat(Lu(o,e,n,s));return r}}function ap(t,e,n,s,i){const r=e._queryIdentifier,o=t.views.get(r);if(!o){let a=Ir(n,i?s:null),l=!1;a?l=!0:s instanceof W?(a=hl(n,s),l=!1):(a=W.EMPTY_NODE,l=!1);const c=Jr(new dn(a,l,!1),new dn(s,i,!1));return new Zb(e,c)}return o}function lC(t,e,n,s,i,r){const o=ap(t,e,s,i,r);return t.views.has(e._queryIdentifier)||t.views.set(e._queryIdentifier,o),sC(o,n),iC(o,n)}function cC(t,e,n,s){const i=e._queryIdentifier,r=[];let o=[];const a=fn(t);if(i==="default")for(const[l,c]of t.views.entries())o=o.concat(Mu(c,n,s)),Du(c)&&(t.views.delete(l),c.query._queryParams.loadsAllData()||r.push(c.query));else{const l=t.views.get(i);l&&(o=o.concat(Mu(l,n,s)),Du(l)&&(t.views.delete(i),l.query._queryParams.loadsAllData()||r.push(l.query)))}return a&&!fn(t)&&r.push(new(oC())(e._repo,e._path)),{removed:r,events:o}}function lp(t){const e=[];for(const n of t.views.values())n.query._queryParams.loadsAllData()||e.push(n);return e}function un(t,e){let n=null;for(const s of t.views.values())n=n||nC(s,e);return n}function cp(t,e){if(e._queryParams.loadsAllData())return Zr(t);{const s=e._queryIdentifier;return t.views.get(s)}}function up(t,e){return cp(t,e)!=null}function fn(t){return Zr(t)!=null}function Zr(t){for(const e of t.views.values())if(e.query._queryParams.loadsAllData())return e;return null}/**
 * @license
 * Copyright 2017 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */let Tr;function uC(t){C(!Tr,"__referenceConstructor has already been defined"),Tr=t}function hC(){return C(Tr,"Reference.ts has not been loaded"),Tr}let dC=1;class Fu{constructor(e){this.listenProvider_=e,this.syncPointTree_=new pe(null),this.pendingWriteTree_=Vb(),this.tagToQueryMap=new Map,this.queryToTagMap=new Map}}function hp(t,e,n,s,i){return Nb(t.pendingWriteTree_,e,n,s,i),i?Ai(t,new Hn(Qf(),e,n)):[]}function kn(t,e,n=!1){const s=kb(t.pendingWriteTree_,e);if(Ob(t.pendingWriteTree_,e)){let r=new pe(null);return s.snap!=null?r=r.set(ee(),!0):Ge(s.children,o=>{r=r.set(new le(o),!0)}),Ai(t,new Er(s.path,r,n))}else return[]}function Pi(t,e,n){return Ai(t,new Hn(cl(),e,n))}function fC(t,e,n){const s=pe.fromObject(n);return Ai(t,new di(cl(),e,s))}function pC(t,e){return Ai(t,new hi(cl(),e))}function _C(t,e,n){const s=gl(t,n);if(s){const i=ml(s),r=i.path,o=i.queryId,a=Le(r,e),l=new hi(ul(o),a);return vl(t,r,l)}else return[]}function Rr(t,e,n,s,i=!1){const r=e._path,o=t.syncPointTree_.get(r);let a=[];if(o&&(e._queryIdentifier==="default"||up(o,e))){const l=cC(o,e,n,s);aC(o)&&(t.syncPointTree_=t.syncPointTree_.remove(r));const c=l.removed;if(a=l.events,!i){const h=c.findIndex(d=>d._queryParams.loadsAllData())!==-1,u=t.syncPointTree_.findOnPath(r,(d,_)=>fn(_));if(h&&!u){const d=t.syncPointTree_.subtree(r);if(!d.isEmpty()){const _=vC(d);for(let m=0;m<_.length;++m){const I=_[m],O=I.query,D=_p(t,I);t.listenProvider_.startListening(Ys(O),fi(t,O),D.hashFn,D.onComplete)}}}!u&&c.length>0&&!s&&(h?t.listenProvider_.stopListening(Ys(e),null):c.forEach(d=>{const _=t.queryToTagMap.get(eo(d));t.listenProvider_.stopListening(Ys(d),_)}))}yC(t,c)}return a}function dp(t,e,n,s){const i=gl(t,s);if(i!=null){const r=ml(i),o=r.path,a=r.queryId,l=Le(o,e),c=new Hn(ul(a),l,n);return vl(t,o,c)}else return[]}function gC(t,e,n,s){const i=gl(t,s);if(i){const r=ml(i),o=r.path,a=r.queryId,l=Le(o,e),c=pe.fromObject(n),h=new di(ul(a),l,c);return vl(t,o,h)}else return[]}function fa(t,e,n,s=!1){const i=e._path;let r=null,o=!1;t.syncPointTree_.foreachOnPath(i,(d,_)=>{const m=Le(d,i);r=r||un(_,m),o=o||fn(_)});let a=t.syncPointTree_.get(i);a?(o=o||fn(a),r=r||un(a,ee())):(a=new op,t.syncPointTree_=t.syncPointTree_.set(i,a));let l;r!=null?l=!0:(l=!1,r=W.EMPTY_NODE,t.syncPointTree_.subtree(i).foreachChild((_,m)=>{const I=un(m,ee());I&&(r=r.updateImmediateChild(_,I))}));const c=up(a,e);if(!c&&!e._queryParams.loadsAllData()){const d=eo(e);C(!t.queryToTagMap.has(d),"View does not exist, but we have a tag");const _=EC();t.queryToTagMap.set(d,_),t.tagToQueryMap.set(_,d)}const h=Xr(t.pendingWriteTree_,i);let u=lC(a,e,n,h,r,l);if(!c&&!o&&!s){const d=cp(a,e);u=u.concat(wC(t,e,d))}return u}function _l(t,e,n){const i=t.pendingWriteTree_,r=t.syncPointTree_.findOnPath(e,(o,a)=>{const l=Le(o,e),c=un(a,l);if(c)return c});return ep(i,e,r,n,!0)}function mC(t,e){const n=e._path;let s=null;t.syncPointTree_.foreachOnPath(n,(c,h)=>{const u=Le(c,n);s=s||un(h,u)});let i=t.syncPointTree_.get(n);i?s=s||un(i,ee()):(i=new op,t.syncPointTree_=t.syncPointTree_.set(n,i));const r=s!=null,o=r?new dn(s,!0,!1):null,a=Xr(t.pendingWriteTree_,e._path),l=ap(i,e,a,r?o.getNode():W.EMPTY_NODE,r);return tC(l)}function Ai(t,e){return fp(e,t.syncPointTree_,null,Xr(t.pendingWriteTree_,ee()))}function fp(t,e,n,s){if(Q(t.path))return pp(t,e,n,s);{const i=e.get(ee());n==null&&i!=null&&(n=un(i,ee()));let r=[];const o=z(t.path),a=t.operationForChild(o),l=e.children.get(o);if(l&&a){const c=n?n.getImmediateChild(o):null,h=tp(s,o);r=r.concat(fp(a,l,c,h))}return i&&(r=r.concat(pl(i,t,s,n))),r}}function pp(t,e,n,s){const i=e.get(ee());n==null&&i!=null&&(n=un(i,ee()));let r=[];return e.children.inorderTraversal((o,a)=>{const l=n?n.getImmediateChild(o):null,c=tp(s,o),h=t.operationForChild(o);h&&(r=r.concat(pp(h,a,l,c)))}),i&&(r=r.concat(pl(i,t,s,n))),r}function _p(t,e){const n=e.query,s=fi(t,n);return{hashFn:()=>(eC(e)||W.EMPTY_NODE).hash(),onComplete:i=>{if(i==="ok")return s?_C(t,n._path,s):pC(t,n._path);{const r=mI(i,n);return Rr(t,n,null,r)}}}}function fi(t,e){const n=eo(e);return t.queryToTagMap.get(n)}function eo(t){return t._path.toString()+"$"+t._queryIdentifier}function gl(t,e){return t.tagToQueryMap.get(e)}function ml(t){const e=t.indexOf("$");return C(e!==-1&&e<t.length-1,"Bad queryKey."),{queryId:t.substr(e+1),path:new le(t.substr(0,e))}}function vl(t,e,n){const s=t.syncPointTree_.get(e);C(s,"Missing sync point for query tag that we're tracking");const i=Xr(t.pendingWriteTree_,e);return pl(s,n,i,null)}function vC(t){return t.fold((e,n,s)=>{if(n&&fn(n))return[Zr(n)];{let i=[];return n&&(i=lp(n)),Ge(s,(r,o)=>{i=i.concat(o)}),i}})}function Ys(t){return t._queryParams.loadsAllData()&&!t._queryParams.isDefault()?new(hC())(t._repo,t._path):t}function yC(t,e){for(let n=0;n<e.length;++n){const s=e[n];if(!s._queryParams.loadsAllData()){const i=eo(s),r=t.queryToTagMap.get(i);t.queryToTagMap.delete(i),t.tagToQueryMap.delete(r)}}}function EC(){return dC++}function wC(t,e,n){const s=e._path,i=fi(t,e),r=_p(t,n),o=t.listenProvider_.startListening(Ys(e),i,r.hashFn,r.onComplete),a=t.syncPointTree_.subtree(s);if(i)C(!fn(a.value),"If we're adding a query, it shouldn't be shadowed");else{const l=a.fold((c,h,u)=>{if(!Q(c)&&h&&fn(h))return[Zr(h).query];{let d=[];return h&&(d=d.concat(lp(h).map(_=>_.query))),Ge(u,(_,m)=>{d=d.concat(m)}),d}});for(let c=0;c<l.length;++c){const h=l[c];t.listenProvider_.stopListening(Ys(h),fi(t,h))}}return o}/**
 * @license
 * Copyright 2017 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */class yl{constructor(e){this.node_=e}getImmediateChild(e){const n=this.node_.getImmediateChild(e);return new yl(n)}node(){return this.node_}}class El{constructor(e,n){this.syncTree_=e,this.path_=n}getImmediateChild(e){const n=Ie(this.path_,e);return new El(this.syncTree_,n)}node(){return _l(this.syncTree_,this.path_)}}const IC=function(t){return t=t||{},t.timestamp=t.timestamp||new Date().getTime(),t},Uu=function(t,e,n){if(!t||typeof t!="object")return t;if(C(".sv"in t,"Unexpected leaf node or priority contents"),typeof t[".sv"]=="string")return bC(t[".sv"],e,n);if(typeof t[".sv"]=="object")return CC(t[".sv"],e);C(!1,"Unexpected server value: "+JSON.stringify(t,null,2))},bC=function(t,e,n){switch(t){case"timestamp":return n.timestamp;default:C(!1,"Unexpected server value: "+t)}},CC=function(t,e,n){t.hasOwnProperty("increment")||C(!1,"Unexpected server value: "+JSON.stringify(t,null,2));const s=t.increment;typeof s!="number"&&C(!1,"Unexpected increment value: "+s);const i=e.node();if(C(i!==null&&typeof i<"u","Expected ChildrenNode.EMPTY_NODE for nulls"),!i.isLeafNode())return s;const o=i.getValue();return typeof o!="number"?s:o+s},SC=function(t,e,n,s){return wl(e,new El(n,t),s)},gp=function(t,e,n){return wl(t,new yl(e),n)};function wl(t,e,n){const s=t.getPriority().val(),i=Uu(s,e.getImmediateChild(".priority"),n);let r;if(t.isLeafNode()){const o=t,a=Uu(o.getValue(),e,n);return a!==o.getValue()||i!==o.getPriority().val()?new Ce(a,Te(i)):t}else{const o=t;return r=o,i!==o.getPriority().val()&&(r=r.updatePriority(new Ce(i))),o.forEachChild(me,(a,l)=>{const c=wl(l,e.getImmediateChild(a),n);c!==l&&(r=r.updateImmediateChild(a,c))}),r}}/**
 * @license
 * Copyright 2017 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */class Il{constructor(e="",n=null,s={children:{},childCount:0}){this.name=e,this.parent=n,this.node=s}}function bl(t,e){let n=e instanceof le?e:new le(e),s=t,i=z(n);for(;i!==null;){const r=ds(s.node.children,i)||{children:{},childCount:0};s=new Il(i,s,r),n=he(n),i=z(n)}return s}function bs(t){return t.node.value}function mp(t,e){t.node.value=e,pa(t)}function vp(t){return t.node.childCount>0}function TC(t){return bs(t)===void 0&&!vp(t)}function to(t,e){Ge(t.node.children,(n,s)=>{e(new Il(n,t,s))})}function yp(t,e,n,s){n&&e(t),to(t,i=>{yp(i,e,!0)})}function RC(t,e,n){let s=t.parent;for(;s!==null;){if(e(s))return!0;s=s.parent}return!1}function Ni(t){return new le(t.parent===null?t.name:Ni(t.parent)+"/"+t.name)}function pa(t){t.parent!==null&&PC(t.parent,t.name,t)}function PC(t,e,n){const s=TC(n),i=Vt(t.node.children,e);s&&i?(delete t.node.children[e],t.node.childCount--,pa(t)):!s&&!i&&(t.node.children[e]=n.node,t.node.childCount++,pa(t))}/**
 * @license
 * Copyright 2017 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */const AC=/[\[\].#$\/\u0000-\u001F\u007F]/,NC=/[\[\].#$\u0000-\u001F\u007F]/,Oo=10*1024*1024,Ep=function(t){return typeof t=="string"&&t.length!==0&&!AC.test(t)},wp=function(t){return typeof t=="string"&&t.length!==0&&!NC.test(t)},kC=function(t){return t&&(t=t.replace(/^\/*\.info(\/|$)/,"/")),wp(t)},Ip=function(t,e,n,s){s&&e===void 0||Cl(Wa(t,"value"),e,n)},Cl=function(t,e,n){const s=n instanceof le?new QI(n,t):n;if(e===void 0)throw new Error(t+"contains undefined "+Rn(s));if(typeof e=="function")throw new Error(t+"contains a function "+Rn(s)+" with contents = "+e.toString());if(vf(e))throw new Error(t+"contains "+e.toString()+" "+Rn(s));if(typeof e=="string"&&e.length>Oo/3&&Kr(e)>Oo)throw new Error(t+"contains a string greater than "+Oo+" utf8 bytes "+Rn(s)+" ('"+e.substring(0,50)+"...')");if(e&&typeof e=="object"){let i=!1,r=!1;if(Ge(e,(o,a)=>{if(o===".value")i=!0;else if(o!==".priority"&&o!==".sv"&&(r=!0,!Ep(o)))throw new Error(t+" contains an invalid key ("+o+") "+Rn(s)+`.  Keys must be non-empty strings and can't contain ".", "#", "$", "/", "[", or "]"`);JI(s,o),Cl(t,a,s),XI(s)}),i&&r)throw new Error(t+' contains ".value" child '+Rn(s)+" in addition to actual children.")}},bp=function(t,e,n,s){if(!wp(n))throw new Error(Wa(t,e)+'was an invalid path = "'+n+`". Paths must be non-empty strings and can't contain ".", "#", "$", "[", or "]"`)},OC=function(t,e,n,s){n&&(n=n.replace(/^\/*\.info(\/|$)/,"/")),bp(t,e,n)},Sl=function(t,e){if(z(e)===".info")throw new Error(t+" failed = Can't modify data under /.info/")},xC=function(t,e){const n=e.path.toString();if(typeof e.repoInfo.host!="string"||e.repoInfo.host.length===0||!Ep(e.repoInfo.namespace)&&e.repoInfo.host.split(":")[0]!=="localhost"||n.length!==0&&!kC(n))throw new Error(Wa(t,"url")+`must be a valid firebase URL and the path can't contain ".", "#", "$", "[", or "]".`)};/**
 * @license
 * Copyright 2017 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */class DC{constructor(){this.eventLists_=[],this.recursionDepth_=0}}function Tl(t,e){let n=null;for(let s=0;s<e.length;s++){const i=e[s],r=i.getPath();n!==null&&!il(r,n.path)&&(t.eventLists_.push(n),n=null),n===null&&(n={events:[],path:r}),n.events.push(i)}n&&t.eventLists_.push(n)}function Cp(t,e,n){Tl(t,n),Sp(t,s=>il(s,e))}function It(t,e,n){Tl(t,n),Sp(t,s=>rt(s,e)||rt(e,s))}function Sp(t,e){t.recursionDepth_++;let n=!0;for(let s=0;s<t.eventLists_.length;s++){const i=t.eventLists_[s];if(i){const r=i.path;e(r)?(MC(t.eventLists_[s]),t.eventLists_[s]=null):n=!1}}n&&(t.eventLists_=[]),t.recursionDepth_--}function MC(t){for(let e=0;e<t.events.length;e++){const n=t.events[e];if(n!==null){t.events[e]=null;const s=n.getEventRunner();Gs&&Ne("event: "+n.toString()),Is(s)}}}/**
 * @license
 * Copyright 2017 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */const LC="repo_interrupt",FC=25;class UC{constructor(e,n,s,i){this.repoInfo_=e,this.forceRestClient_=n,this.authTokenProvider_=s,this.appCheckProvider_=i,this.dataUpdateCount=0,this.statsListener_=null,this.eventQueue_=new DC,this.nextWriteId_=1,this.interceptServerDataCallback_=null,this.onDisconnect_=yr(),this.transactionQueueTree_=new Il,this.persistentConnection_=null,this.key=this.repoInfo_.toURLString()}toString(){return(this.repoInfo_.secure?"https://":"http://")+this.repoInfo_.host}}function WC(t,e,n){if(t.stats_=nl(t.repoInfo_),t.forceRestClient_||wI())t.server_=new vr(t.repoInfo_,(s,i,r,o)=>{Wu(t,s,i,r,o)},t.authTokenProvider_,t.appCheckProvider_),setTimeout(()=>Hu(t,!0),0);else{if(typeof n<"u"&&n!==null){if(typeof n!="object")throw new Error("Only objects are supported for option databaseAuthVariableOverride");try{we(n)}catch(s){throw new Error("Invalid authOverride provided: "+s)}}t.persistentConnection_=new Lt(t.repoInfo_,e,(s,i,r,o)=>{Wu(t,s,i,r,o)},s=>{Hu(t,s)},s=>{HC(t,s)},t.authTokenProvider_,t.appCheckProvider_,n),t.server_=t.persistentConnection_}t.authTokenProvider_.addTokenChangeListener(s=>{t.server_.refreshAuthToken(s)}),t.appCheckProvider_.addTokenChangeListener(s=>{t.server_.refreshAppCheckToken(s.token)}),t.statsReporter_=TI(t.repoInfo_,()=>new Cb(t.stats_,t.server_)),t.infoData_=new yb,t.infoSyncTree_=new Fu({startListening:(s,i,r,o)=>{let a=[];const l=t.infoData_.getNode(s._path);return l.isEmpty()||(a=Pi(t.infoSyncTree_,s._path,l),setTimeout(()=>{o("ok")},0)),a},stopListening:()=>{}}),Pl(t,"connected",!1),t.serverSyncTree_=new Fu({startListening:(s,i,r,o)=>(t.server_.listen(s,r,i,(a,l)=>{const c=o(a,l);It(t.eventQueue_,s._path,c)}),[]),stopListening:(s,i)=>{t.server_.unlisten(s,i)}})}function Tp(t){const n=t.infoData_.getNode(new le(".info/serverTimeOffset")).val()||0;return new Date().getTime()+n}function Rl(t){return IC({timestamp:Tp(t)})}function Wu(t,e,n,s,i){t.dataUpdateCount++;const r=new le(e);n=t.interceptServerDataCallback_?t.interceptServerDataCallback_(e,n):n;let o=[];if(i)if(s){const l=ar(n,c=>Te(c));o=gC(t.serverSyncTree_,r,l,i)}else{const l=Te(n);o=dp(t.serverSyncTree_,r,l,i)}else if(s){const l=ar(n,c=>Te(c));o=fC(t.serverSyncTree_,r,l)}else{const l=Te(n);o=Pi(t.serverSyncTree_,r,l)}let a=r;o.length>0&&(a=so(t,r)),It(t.eventQueue_,a,o)}function Hu(t,e){Pl(t,"connected",e),e===!1&&$C(t)}function HC(t,e){Ge(e,(n,s)=>{Pl(t,n,s)})}function Pl(t,e,n){const s=new le("/.info/"+e),i=Te(n);t.infoData_.updateSnapshot(s,i);const r=Pi(t.infoSyncTree_,s,i);It(t.eventQueue_,s,r)}function Rp(t){return t.nextWriteId_++}function VC(t,e,n){const s=mC(t.serverSyncTree_,e);return s!=null?Promise.resolve(s):t.server_.get(e).then(i=>{const r=Te(i).withIndex(e._queryParams.getIndex());fa(t.serverSyncTree_,e,n,!0);let o;if(e._queryParams.loadsAllData())o=Pi(t.serverSyncTree_,e._path,r);else{const a=fi(t.serverSyncTree_,e);o=dp(t.serverSyncTree_,e._path,r,a)}return It(t.eventQueue_,e._path,o),Rr(t.serverSyncTree_,e,n,null,!0),r},i=>(no(t,"get for query "+we(e)+" failed: "+i),Promise.reject(new Error(i))))}function BC(t,e,n,s,i){no(t,"set",{path:e.toString(),value:n,priority:s});const r=Rl(t),o=Te(n,s),a=_l(t.serverSyncTree_,e),l=gp(o,a,r),c=Rp(t),h=hp(t.serverSyncTree_,e,l,c,!0);Tl(t.eventQueue_,h),t.server_.put(e.toString(),o.val(!0),(d,_)=>{const m=d==="ok";m||je("set at "+e+" failed: "+d);const I=kn(t.serverSyncTree_,c,!m);It(t.eventQueue_,e,I),qC(t,i,d,_)});const u=Op(t,e);so(t,u),It(t.eventQueue_,u,[])}function $C(t){no(t,"onDisconnectEvents");const e=Rl(t),n=yr();la(t.onDisconnect_,ee(),(i,r)=>{const o=SC(i,r,t.serverSyncTree_,e);Yf(n,i,o)});let s=[];la(n,ee(),(i,r)=>{s=s.concat(Pi(t.serverSyncTree_,i,r));const o=Op(t,i);so(t,o)}),t.onDisconnect_=yr(),It(t.eventQueue_,ee(),s)}function jC(t,e,n){let s;z(e._path)===".info"?s=fa(t.infoSyncTree_,e,n):s=fa(t.serverSyncTree_,e,n),Cp(t.eventQueue_,e._path,s)}function GC(t,e,n){let s;z(e._path)===".info"?s=Rr(t.infoSyncTree_,e,n):s=Rr(t.serverSyncTree_,e,n),Cp(t.eventQueue_,e._path,s)}function KC(t){t.persistentConnection_&&t.persistentConnection_.interrupt(LC)}function no(t,...e){let n="";t.persistentConnection_&&(n=t.persistentConnection_.id+":"),Ne(n,...e)}function qC(t,e,n,s){e&&Is(()=>{if(n==="ok")e(null);else{const i=(n||"error").toUpperCase();let r=i;s&&(r+=": "+s);const o=new Error(r);o.code=i,e(o)}})}function Pp(t,e,n){return _l(t.serverSyncTree_,e,n)||W.EMPTY_NODE}function Al(t,e=t.transactionQueueTree_){if(e||io(t,e),bs(e)){const n=Np(t,e);C(n.length>0,"Sending zero length transaction queue"),n.every(i=>i.status===0)&&zC(t,Ni(e),n)}else vp(e)&&to(e,n=>{Al(t,n)})}function zC(t,e,n){const s=n.map(c=>c.currentWriteId),i=Pp(t,e,s);let r=i;const o=i.hash();for(let c=0;c<n.length;c++){const h=n[c];C(h.status===0,"tryToSendTransactionQueue_: items in queue should all be run."),h.status=1,h.retryCount++;const u=Le(e,h.path);r=r.updateChild(u,h.currentOutputSnapshotRaw)}const a=r.val(!0),l=e;t.server_.put(l.toString(),a,c=>{no(t,"transaction put response",{path:l.toString(),status:c});let h=[];if(c==="ok"){const u=[];for(let d=0;d<n.length;d++)n[d].status=2,h=h.concat(kn(t.serverSyncTree_,n[d].currentWriteId)),n[d].onComplete&&u.push(()=>n[d].onComplete(null,!0,n[d].currentOutputSnapshotResolved)),n[d].unwatcher();io(t,bl(t.transactionQueueTree_,e)),Al(t,t.transactionQueueTree_),It(t.eventQueue_,e,h);for(let d=0;d<u.length;d++)Is(u[d])}else{if(c==="datastale")for(let u=0;u<n.length;u++)n[u].status===3?n[u].status=4:n[u].status=0;else{je("transaction at "+l.toString()+" failed: "+c);for(let u=0;u<n.length;u++)n[u].status=4,n[u].abortReason=c}so(t,e)}},o)}function so(t,e){const n=Ap(t,e),s=Ni(n),i=Np(t,n);return YC(t,i,s),s}function YC(t,e,n){if(e.length===0)return;const s=[];let i=[];const o=e.filter(a=>a.status===0).map(a=>a.currentWriteId);for(let a=0;a<e.length;a++){const l=e[a],c=Le(n,l.path);let h=!1,u;if(C(c!==null,"rerunTransactionsUnderNode_: relativePath should not be null."),l.status===4)h=!0,u=l.abortReason,i=i.concat(kn(t.serverSyncTree_,l.currentWriteId,!0));else if(l.status===0)if(l.retryCount>=FC)h=!0,u="maxretry",i=i.concat(kn(t.serverSyncTree_,l.currentWriteId,!0));else{const d=Pp(t,l.path,o);l.currentInputSnapshot=d;const _=e[a].update(d.val());if(_!==void 0){Cl("transaction failed: Data returned ",_,l.path);let m=Te(_);typeof _=="object"&&_!=null&&Vt(_,".priority")||(m=m.updatePriority(d.getPriority()));const O=l.currentWriteId,D=Rl(t),k=gp(m,d,D);l.currentOutputSnapshotRaw=m,l.currentOutputSnapshotResolved=k,l.currentWriteId=Rp(t),o.splice(o.indexOf(O),1),i=i.concat(hp(t.serverSyncTree_,l.path,k,l.currentWriteId,l.applyLocally)),i=i.concat(kn(t.serverSyncTree_,O,!0))}else h=!0,u="nodata",i=i.concat(kn(t.serverSyncTree_,l.currentWriteId,!0))}It(t.eventQueue_,n,i),i=[],h&&(e[a].status=2,function(d){setTimeout(d,Math.floor(0))}(e[a].unwatcher),e[a].onComplete&&(u==="nodata"?s.push(()=>e[a].onComplete(null,!1,e[a].currentInputSnapshot)):s.push(()=>e[a].onComplete(new Error(u),!1,null))))}io(t,t.transactionQueueTree_);for(let a=0;a<s.length;a++)Is(s[a]);Al(t,t.transactionQueueTree_)}function Ap(t,e){let n,s=t.transactionQueueTree_;for(n=z(e);n!==null&&bs(s)===void 0;)s=bl(s,n),e=he(e),n=z(e);return s}function Np(t,e){const n=[];return kp(t,e,n),n.sort((s,i)=>s.order-i.order),n}function kp(t,e,n){const s=bs(e);if(s)for(let i=0;i<s.length;i++)n.push(s[i]);to(e,i=>{kp(t,i,n)})}function io(t,e){const n=bs(e);if(n){let s=0;for(let i=0;i<n.length;i++)n[i].status!==2&&(n[s]=n[i],s++);n.length=s,mp(e,n.length>0?n:void 0)}to(e,s=>{io(t,s)})}function Op(t,e){const n=Ni(Ap(t,e)),s=bl(t.transactionQueueTree_,e);return RC(s,i=>{xo(t,i)}),xo(t,s),yp(s,i=>{xo(t,i)}),n}function xo(t,e){const n=bs(e);if(n){const s=[];let i=[],r=-1;for(let o=0;o<n.length;o++)n[o].status===3||(n[o].status===1?(C(r===o-1,"All SENT items should be at beginning of queue."),r=o,n[o].status=3,n[o].abortReason="set"):(C(n[o].status===0,"Unexpected transaction status in abort"),n[o].unwatcher(),i=i.concat(kn(t.serverSyncTree_,n[o].currentWriteId,!0)),n[o].onComplete&&s.push(n[o].onComplete.bind(null,new Error("set"),!1,null))));r===-1?mp(e,void 0):n.length=r+1,It(t.eventQueue_,Ni(e),i);for(let o=0;o<s.length;o++)Is(s[o])}}/**
 * @license
 * Copyright 2017 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */function QC(t){let e="";const n=t.split("/");for(let s=0;s<n.length;s++)if(n[s].length>0){let i=n[s];try{i=decodeURIComponent(i.replace(/\+/g," "))}catch{}e+="/"+i}return e}function JC(t){const e={};t.charAt(0)==="?"&&(t=t.substring(1));for(const n of t.split("&")){if(n.length===0)continue;const s=n.split("=");s.length===2?e[decodeURIComponent(s[0])]=decodeURIComponent(s[1]):je(`Invalid query segment '${n}' in query '${t}'`)}return e}const Vu=function(t,e){const n=XC(t),s=n.namespace;n.domain==="firebase.com"&&Wt(n.host+" is no longer supported. Please use <YOUR FIREBASE>.firebaseio.com instead"),(!s||s==="undefined")&&n.domain!=="localhost"&&Wt("Cannot parse Firebase url. Please use https://<YOUR FIREBASE>.firebaseio.com"),n.secure||dI();const i=n.scheme==="ws"||n.scheme==="wss";return{repoInfo:new Nf(n.host,n.secure,s,i,e,"",s!==n.subdomain),path:new le(n.pathString)}},XC=function(t){let e="",n="",s="",i="",r="",o=!0,a="https",l=443;if(typeof t=="string"){let c=t.indexOf("//");c>=0&&(a=t.substring(0,c-1),t=t.substring(c+2));let h=t.indexOf("/");h===-1&&(h=t.length);let u=t.indexOf("?");u===-1&&(u=t.length),e=t.substring(0,Math.min(h,u)),h<u&&(i=QC(t.substring(h,u)));const d=JC(t.substring(Math.min(t.length,u)));c=e.indexOf(":"),c>=0?(o=a==="https"||a==="wss",l=parseInt(e.substring(c+1),10)):c=e.length;const _=e.slice(0,c);if(_.toLowerCase()==="localhost")n="localhost";else if(_.split(".").length<=2)n=_;else{const m=e.indexOf(".");s=e.substring(0,m).toLowerCase(),n=e.substring(m+1),r=s}"ns"in d&&(r=d.ns)}return{host:e,port:l,domain:n,subdomain:s,secure:o,scheme:a,pathString:i,namespace:r}};/**
 * @license
 * Copyright 2017 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */const Bu="-0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz",ZC=function(){let t=0;const e=[];return function(n){const s=n===t;t=n;let i;const r=new Array(8);for(i=7;i>=0;i--)r[i]=Bu.charAt(n%64),n=Math.floor(n/64);C(n===0,"Cannot push at time == 0");let o=r.join("");if(s){for(i=11;i>=0&&e[i]===63;i--)e[i]=0;e[i]++}else for(i=0;i<12;i++)e[i]=Math.floor(Math.random()*64);for(i=0;i<12;i++)o+=Bu.charAt(e[i]);return C(o.length===20,"nextPushId: Length should be 20."),o}}();/**
 * @license
 * Copyright 2017 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */class eS{constructor(e,n,s,i){this.eventType=e,this.eventRegistration=n,this.snapshot=s,this.prevName=i}getPath(){const e=this.snapshot.ref;return this.eventType==="value"?e._path:e.parent._path}getEventType(){return this.eventType}getEventRunner(){return this.eventRegistration.getEventRunner(this)}toString(){return this.getPath().toString()+":"+this.eventType+":"+we(this.snapshot.exportVal())}}class tS{constructor(e,n,s){this.eventRegistration=e,this.error=n,this.path=s}getPath(){return this.path}getEventType(){return"cancel"}getEventRunner(){return this.eventRegistration.getEventRunner(this)}toString(){return this.path.toString()+":cancel"}}/**
 * @license
 * Copyright 2017 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */class xp{constructor(e,n){this.snapshotCallback=e,this.cancelCallback=n}onValue(e,n){this.snapshotCallback.call(null,e,n)}onCancel(e){return C(this.hasCancelCallback,"Raising a cancel event on a listener with no cancel callback"),this.cancelCallback.call(null,e)}get hasCancelCallback(){return!!this.cancelCallback}matches(e){return this.snapshotCallback===e.snapshotCallback||this.snapshotCallback.userCallback!==void 0&&this.snapshotCallback.userCallback===e.snapshotCallback.userCallback&&this.snapshotCallback.context===e.snapshotCallback.context}}/**
 * @license
 * Copyright 2020 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */class Nl{constructor(e,n,s,i){this._repo=e,this._path=n,this._queryParams=s,this._orderByCalled=i}get key(){return Q(this._path)?null:Wf(this._path)}get ref(){return new Bt(this._repo,this._path)}get _queryIdentifier(){const e=Tu(this._queryParams),n=el(e);return n==="{}"?"default":n}get _queryObject(){return Tu(this._queryParams)}isEqual(e){if(e=Ue(e),!(e instanceof Nl))return!1;const n=this._repo===e._repo,s=il(this._path,e._path),i=this._queryIdentifier===e._queryIdentifier;return n&&s&&i}toJSON(){return this.toString()}toString(){return this._repo.toString()+YI(this._path)}}class Bt extends Nl{constructor(e,n){super(e,n,new ll,!1)}get parent(){const e=Vf(this._path);return e===null?null:new Bt(this._repo,e)}get root(){let e=this;for(;e.parent!==null;)e=e.parent;return e}}class pi{constructor(e,n,s){this._node=e,this.ref=n,this._index=s}get priority(){return this._node.getPriority().val()}get key(){return this.ref.key}get size(){return this._node.numChildren()}child(e){const n=new le(e),s=_i(this.ref,e);return new pi(this._node.getChild(n),s,me)}exists(){return!this._node.isEmpty()}exportVal(){return this._node.val(!0)}forEach(e){return this._node.isLeafNode()?!1:!!this._node.forEachChild(this._index,(s,i)=>e(new pi(i,_i(this.ref,s),me)))}hasChild(e){const n=new le(e);return!this._node.getChild(n).isEmpty()}hasChildren(){return this._node.isLeafNode()?!1:!this._node.isEmpty()}toJSON(){return this.exportVal()}val(){return this._node.val()}}function qS(t,e){return t=Ue(t),t._checkNotDeleted("ref"),e!==void 0?_i(t._root,e):t._root}function _i(t,e){return t=Ue(t),z(t._path)===null?OC("child","path",e):bp("child","path",e),new Bt(t._repo,Ie(t._path,e))}function zS(t,e){t=Ue(t),Sl("push",t._path),Ip("push",e,t._path,!0);const n=Tp(t._repo),s=ZC(n),i=_i(t,s),r=_i(t,s);let o;return o=Promise.resolve(r),i.then=o.then.bind(o),i.catch=o.then.bind(o,void 0),i}function YS(t){return Sl("remove",t._path),nS(t,null)}function nS(t,e){t=Ue(t),Sl("set",t._path),Ip("set",e,t._path,!1);const n=new Gr;return BC(t._repo,t._path,e,null,n.wrapCallback(()=>{})),n.promise}function QS(t){t=Ue(t);const e=new xp(()=>{}),n=new ro(e);return VC(t._repo,t,n).then(s=>new pi(s,new Bt(t._repo,t._path),t._queryParams.getIndex()))}class ro{constructor(e){this.callbackContext=e}respondsTo(e){return e==="value"}createEvent(e,n){const s=n._queryParams.getIndex();return new eS("value",this,new pi(e.snapshotNode,new Bt(n._repo,n._path),s))}getEventRunner(e){return e.getEventType()==="cancel"?()=>this.callbackContext.onCancel(e.error):()=>this.callbackContext.onValue(e.snapshot,null)}createCancelEvent(e,n){return this.callbackContext.hasCancelCallback?new tS(this,e,n):null}matches(e){return e instanceof ro?!e.callbackContext||!this.callbackContext?!0:e.callbackContext.matches(this.callbackContext):!1}hasAnyCallback(){return this.callbackContext!==null}}function sS(t,e,n,s,i){const r=new xp(n,void 0),o=new ro(r);return jC(t._repo,t,o),()=>GC(t._repo,t,o)}function JS(t,e,n,s){return sS(t,"value",e)}rC(Bt);uC(Bt);/**
 * @license
 * Copyright 2020 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */const iS="FIREBASE_DATABASE_EMULATOR_HOST",_a={};let rS=!1;function oS(t,e,n,s){t.repoInfo_=new Nf(`${e}:${n}`,!1,t.repoInfo_.namespace,t.repoInfo_.webSocketOnly,t.repoInfo_.nodeAdmin,t.repoInfo_.persistenceKey,t.repoInfo_.includeNamespaceInQueryParams,!0),s&&(t.authTokenProvider_=s)}function aS(t,e,n,s,i){let r=s||t.options.databaseURL;r===void 0&&(t.options.projectId||Wt("Can't determine Firebase Database URL. Be sure to include  a Project ID when calling firebase.initializeApp()."),Ne("Using default host for project ",t.options.projectId),r=`${t.options.projectId}-default-rtdb.firebaseio.com`);let o=Vu(r,i),a=o.repoInfo,l;typeof process<"u"&&lu&&(l=lu[iS]),l?(r=`http://${l}?ns=${a.namespace}`,o=Vu(r,i),a=o.repoInfo):o.repoInfo.secure;const c=new bI(t.name,t.options,e);xC("Invalid Firebase Database URL",o),Q(o.path)||Wt("Database URL must point to the root of a Firebase Database (not including a child path).");const h=cS(a,t,c,new II(t.name,n));return new uS(h,t)}function lS(t,e){const n=_a[e];(!n||n[t.key]!==t)&&Wt(`Database ${e}(${t.repoInfo_}) has already been deleted.`),KC(t),delete n[t.key]}function cS(t,e,n,s){let i=_a[e.name];i||(i={},_a[e.name]=i);let r=i[t.toURLString()];return r&&Wt("Database initialized multiple times. Please make sure the format of the database URL matches with each database() call."),r=new UC(t,rS,n,s),i[t.toURLString()]=r,r}class uS{constructor(e,n){this._repoInternal=e,this.app=n,this.type="database",this._instanceStarted=!1}get _repo(){return this._instanceStarted||(WC(this._repoInternal,this.app.options.appId,this.app.options.databaseAuthVariableOverride),this._instanceStarted=!0),this._repoInternal}get _root(){return this._rootInternal||(this._rootInternal=new Bt(this._repo,ee())),this._rootInternal}_delete(){return this._rootInternal!==null&&(lS(this._repo,this.app.name),this._repoInternal=null,this._rootInternal=null),Promise.resolve()}_checkNotDeleted(e){this._rootInternal===null&&Wt("Cannot call "+e+" on a deleted database.")}}function hS(t=Od(),e){const n=Ba(t,"database").getImmediate({identifier:e});if(!n._instanceStarted){const s=bv("database");s&&dS(n,...s)}return n}function dS(t,e,n,s={}){t=Ue(t),t._checkNotDeleted("useEmulator"),t._instanceStarted&&Wt("Cannot call useEmulator() after instance has already been initialized.");const i=t._repoInternal;let r;if(i.repoInfo_.nodeAdmin)s.mockUserToken&&Wt('mockUserToken is not supported by the Admin SDK. For client access with mock users, please use the "firebase" package instead of "firebase-admin".'),r=new qi(qi.OWNER);else if(s.mockUserToken){const o=typeof s.mockUserToken=="string"?s.mockUserToken:Cv(s.mockUserToken,t.app.options.projectId);r=new qi(o)}oS(i,e,n,r)}/**
 * @license
 * Copyright 2021 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */function fS(t){oI(ys),fs(new Ln("database",(e,{instanceIdentifier:n})=>{const s=e.getProvider("app").getImmediate(),i=e.getProvider("auth-internal"),r=e.getProvider("app-check-internal");return aS(s,i,r,n)},"PUBLIC").setMultipleInstances(!0)),ln(cu,uu,t),ln(cu,uu,"esm2017")}Lt.prototype.simpleListen=function(t,e){this.sendRequest("q",{p:t},e)};Lt.prototype.echo=function(t,e){this.sendRequest("echo",{d:t},e)};fS();const pS={apiKey:"AIzaSyCF6b0rG26cqljudVDw13eH9Q9FyAyzVfc",authDomain:"vigilance-shield.firebaseapp.com",databaseURL:"https://vigilance-shield-default-rtdb.firebaseio.com",projectId:"vigilance-shield",storageBucket:"vigilance-shield.firebasestorage.app",messagingSenderId:"771024278683",appId:"1:771024278683:web:5dffc3102fa3b1a67ee23e"},Dp=kd(pS),$u=iI(Dp),XS=hS(Dp),kl=um("auth",{state:()=>({user:null,loading:!0}),getters:{isAuthenticated:t=>!!t.user,userProfile:t=>t.user},actions:{init(){zE($u,t=>{this.user=t,this.loading=!1})},async logout(){try{await $u.signOut(),this.user=null}catch(t){throw console.error("Error logging out:",t),t}}}}),_S={class:"min-h-screen bg-[#EDE8F5]"},gS={key:0,class:"bg-[#3D52A0] shadow-lg"},mS={class:"max-w-7xl mx-auto px-4"},vS={class:"flex justify-between h-16 items-center"},yS={class:"flex items-center"},ES={class:"hidden sm:ml-6 sm:flex sm:space-x-8"},wS={class:"flex items-center sm:hidden"},IS={key:0,class:"h-7 w-7",fill:"none",stroke:"currentColor",viewBox:"0 0 24 24"},bS={key:1,class:"h-7 w-7",fill:"none",stroke:"currentColor",viewBox:"0 0 24 24"},CS={key:0,class:"sm:hidden bg-[#3D52A0] rounded-b-lg shadow-lg mt-2"},SS={class:"flex flex-col space-y-2 p-4"},TS={class:"max-w-7xl mx-auto py-6 sm:px-6 lg:px-8"},RS={key:1,class:"flex justify-center items-center h-screen"},PS={__name:"App",setup(t){const e=pv(),n=kl(),s=Mr(!1),i=async()=>{try{await n.logout(),e.push("/login")}catch(r){console.error("Error logging out:",r)}};return(r,o)=>{const a=Hl("router-link"),l=Hl("router-view");return qt(),qn("div",_S,[rn(n).isAuthenticated?(qt(),qn("nav",gS,[be("div",mS,[be("div",vS,[be("div",yS,[o[7]||(o[7]=be("div",{class:"flex-shrink-0 flex items-center"},[be("h1",{class:"text-xl font-bold text-white"},"VIGILERT")],-1)),be("div",ES,[Ee(a,{to:"/dashboard",class:"nav-link"},{default:Cn(()=>o[4]||(o[4]=[Sn("Dashboard")])),_:1}),Ee(a,{to:"/trip-history",class:"nav-link"},{default:Cn(()=>o[5]||(o[5]=[Sn("Trip History")])),_:1}),Ee(a,{to:"/emergency-contacts",class:"nav-link"},{default:Cn(()=>o[6]||(o[6]=[Sn("Emergency Contacts")])),_:1})])]),be("div",wS,[be("button",{onClick:o[0]||(o[0]=c=>s.value=!s.value),class:"text-white focus:outline-none"},[s.value?(qt(),qn("svg",bS,o[9]||(o[9]=[be("path",{"stroke-linecap":"round","stroke-linejoin":"round","stroke-width":"2",d:"M6 18L18 6M6 6l12 12"},null,-1)]))):(qt(),qn("svg",IS,o[8]||(o[8]=[be("path",{"stroke-linecap":"round","stroke-linejoin":"round","stroke-width":"2",d:"M4 6h16M4 12h16M4 18h16"},null,-1)])))])]),be("div",{class:"hidden sm:flex items-center"},[be("button",{onClick:i,class:"bg-[#7091E6] text-white font-semibold px-4 py-2 rounded-lg shadow transition hover:bg-[#8697C4] hover:scale-105 focus:outline-none focus:ring-2 focus:ring-[#ADBBD4]"},"Logout")])]),s.value?(qt(),qn("div",CS,[be("div",SS,[Ee(a,{to:"/dashboard",class:"nav-link-mobile",onClick:o[1]||(o[1]=c=>s.value=!1)},{default:Cn(()=>o[10]||(o[10]=[Sn("Dashboard")])),_:1}),Ee(a,{to:"/trip-history",class:"nav-link-mobile",onClick:o[2]||(o[2]=c=>s.value=!1)},{default:Cn(()=>o[11]||(o[11]=[Sn("Trip History")])),_:1}),Ee(a,{to:"/emergency-contacts",class:"nav-link-mobile",onClick:o[3]||(o[3]=c=>s.value=!1)},{default:Cn(()=>o[12]||(o[12]=[Sn("Emergency Contacts")])),_:1}),be("button",{onClick:i,class:"bg-[#7091E6] text-white font-semibold px-4 py-2 rounded-lg shadow transition hover:bg-[#8697C4] hover:scale-105 focus:outline-none focus:ring-2 focus:ring-[#ADBBD4] mt-2"},"Logout")])])):Jl("",!0)])])):Jl("",!0),be("main",TS,[rn(n).loading?(qt(),qn("div",RS,o[13]||(o[13]=[be("div",{class:"animate-spin rounded-full h-12 w-12 border-b-2 border-[#7091E6]"},null,-1)]))):(qt(),Jh(l,{key:0}))])])}}},AS="modulepreload",NS=function(t){return"/"+t},ju={},Tt=function(e,n,s){let i=Promise.resolve();if(n&&n.length>0){document.getElementsByTagName("link");const o=document.querySelector("meta[property=csp-nonce]"),a=(o==null?void 0:o.nonce)||(o==null?void 0:o.getAttribute("nonce"));i=Promise.allSettled(n.map(l=>{if(l=NS(l),l in ju)return;ju[l]=!0;const c=l.endsWith(".css"),h=c?'[rel="stylesheet"]':"";if(document.querySelector(`link[href="${l}"]${h}`))return;const u=document.createElement("link");if(u.rel=c?"stylesheet":AS,c||(u.as="script"),u.crossOrigin="",u.href=l,a&&u.setAttribute("nonce",a),document.head.appendChild(u),c)return new Promise((d,_)=>{u.addEventListener("load",d),u.addEventListener("error",()=>_(new Error(`Unable to preload CSS for ${l}`)))})}))}function r(o){const a=new Event("vite:preloadError",{cancelable:!0});if(a.payload=o,window.dispatchEvent(a),!a.defaultPrevented)throw o}return i.then(o=>{for(const a of o||[])a.status==="rejected"&&r(a.reason);return e().catch(r)})},kS=[{path:"/",name:"Home",component:()=>Tt(()=>import("./Home-t2iST9xK.js"),[]),meta:{requiresAuth:!0}},{path:"/login",name:"Login",component:()=>Tt(()=>import("./Login-BKHKko0x.js"),[])},{path:"/register",name:"Register",component:()=>Tt(()=>import("./Register-wGJ1e8Vy.js"),[])},{path:"/dashboard",name:"Dashboard",component:()=>Tt(()=>import("./Dashboard-BWv6PRyz.js"),__vite__mapDeps([0,1,2])),meta:{requiresAuth:!0}},{path:"/trip-history",name:"TripHistory",component:()=>Tt(()=>import("./TripHistory-CYG1zjAA.js"),[]),meta:{requiresAuth:!0}},{path:"/emergency-contacts",name:"EmergencyContacts",component:()=>Tt(()=>import("./EmergencyContacts-lB9xAKXW.js"),[]),meta:{requiresAuth:!0}},{path:"/emergency-login",name:"EmergencyContactLogin",component:()=>Tt(()=>import("./EmergencyContactLogin-DG2UsnGT.js"),[])},{path:"/emergency-dashboard/:userId",name:"EmergencyContactDashboard",component:()=>Tt(()=>import("./EmergencyContactDashboard-CL4KWl9d.js"),__vite__mapDeps([3,1]))}],Mp=dv({history:Vm(),routes:kS});Mp.beforeEach(async(t,e,n)=>{const s=kl();s.loading&&await new Promise(r=>{const o=ns(()=>s.loading,a=>{a||(o(),r())})});const i=t.matched.some(r=>r.meta.requiresAuth);i&&!s.isAuthenticated?n("/login"):!i&&s.isAuthenticated?n("/dashboard"):n()});function OS(t={}){const{immediate:e=!1,onNeedRefresh:n,onOfflineReady:s,onRegistered:i,onRegisteredSW:r,onRegisterError:o}=t;let a,l;const c=async(u=!0)=>{await l};async function h(){if("serviceWorker"in navigator){if(a=await Tt(async()=>{const{Workbox:u}=await import("./workbox-window.prod.es5-B9K5rw8f.js");return{Workbox:u}},[]).then(({Workbox:u})=>new u("/sw.js",{scope:"/",type:"classic"})).catch(u=>{o==null||o(u)}),!a)return;a.addEventListener("activated",u=>{(u.isUpdate||u.isExternal)&&window.location.reload()}),a.addEventListener("installed",u=>{u.isUpdate||s==null||s()}),a.register({immediate:e}).then(u=>{r?r("/sw.js",u):i==null||i(u)}).catch(u=>{o==null||o(u)})}}return l=h(),c}OS({onNeedRefresh(){console.log("A new version of the app is available. Please refresh.")},onOfflineReady(){console.log("The app is ready to work offline.")}});const Ol=tm(PS),xS=im();Ol.use(xS);Ol.use(Mp);const DS=kl();DS.init();Ol.mount("#app");export{QS as $,Aa as A,Fr as B,Wi as C,U_ as D,td as E,Pt as F,Zt as G,mi as H,Xe as I,ns as J,Ph as K,BS as L,Jh as M,Jl as N,rn as O,g_ as P,kg as Q,Z as R,Pa as S,kl as T,JS as U,HS as V,zE as W,zS as X,YS as Y,hS as Z,Tt as _,be as a,$S as a0,LS as a1,qt as b,qn as c,Sn as d,US as e,MS as f,Ee as g,Cn as h,Hl as i,$u as j,GS as k,jS as l,nS as m,qS as n,M_ as o,XS as p,FS as q,Mr as r,KS as s,Yp as t,pv as u,WS as v,VS as w,wa as x,Ch as y,et as z};

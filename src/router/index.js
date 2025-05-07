import { createRouter, createWebHistory } from 'vue-router';
import { useAuthStore } from '../stores/auth';
import { watch } from 'vue';

const routes = [
  {
    path: '/',
    name: 'Home',
    component: () => import('../views/Home.vue'),
    meta: { requiresAuth: true }
  },
  {
    path: '/login',
    name: 'Login',
    component: () => import('../views/Login.vue')
  },
  {
    path: '/register',
    name: 'Register',
    component: () => import('../views/Register.vue')
  },
  {
    path: '/dashboard',
    name: 'Dashboard',
    component: () => import('../views/Dashboard.vue'),
    meta: { requiresAuth: true }
  },
  {
    path: '/trip-history',
    name: 'TripHistory',
    component: () => import('../views/TripHistory.vue'),
    meta: { requiresAuth: true }
  },
  {
    path: '/emergency-contacts',
    name: 'EmergencyContacts',
    component: () => import('../views/EmergencyContacts.vue'),
    meta: { requiresAuth: true }
  },
  {
    path: '/emergency-login',
    name: 'EmergencyContactLogin',
    component: () => import('../views/EmergencyContactLogin.vue')
  },
  {
    path: '/emergency-dashboard/:userId',
    name: 'EmergencyContactDashboard',
    component: () => import('../views/EmergencyContactDashboard.vue')
  }
];

const router = createRouter({
  history: createWebHistory(),
  routes
});

router.beforeEach(async (to, from, next) => {
  const authStore = useAuthStore();
  
  // Wait for auth state to be initialized
  if (authStore.loading) {
    await new Promise(resolve => {
      const unwatch = watch(() => authStore.loading, (loading) => {
        if (!loading) {
          unwatch();
          resolve();
        }
      });
    });
  }

  const requiresAuth = to.matched.some(record => record.meta.requiresAuth);

  if (requiresAuth && !authStore.isAuthenticated) {
    next('/login');
  } else if (!requiresAuth && authStore.isAuthenticated) {
    next('/dashboard');
  } else {
    next();
  }
});

export default router; 
/**********************************************************************/
/** @note initialize system log
 * 
 * @param               pForkName       (in):   current process name 
 * @return              void
 *  */
void initSyslog( zsint8 * pForkName )
{
#if ! defined( ENABLE_DBG_PRINT )
        if ( FALSE == SFW_DEBUG_EN )
        {
                openlog( pForkName, LOG_CONS | LOG_PID, 0 );
                syslog( LOG_INFO, "%s daemon program's pid=%d\n", pForkName, getpid() );
        }
#endif
}
/**********************************************************************/
/** @note display data which is string
 * 
 * @param               pData   (in):   data entry
 * @return              void
 *  */
void dispDbgFmt( const zsint8 * format, ... )
{
#if defined( ENABLE_DBG_PRINT )
        if ( TRUE == SFW_DEBUG_EN )
        {
                va_list arg_ptr;
                va_start( arg_ptr, format );
                vfprintf( stderr, format, arg_ptr );
                va_end( arg_ptr );
        }
#endif
}

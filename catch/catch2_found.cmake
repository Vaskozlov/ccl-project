  enable_testing()

  file(
          GLOB_RECURSE
          CCL_CATCH_OBJECTS
          "catch/*.cpp"
  )

  add_executable(
          ccl-catch
          ${CCL_CATCH_OBJECTS}
  )

  target_link_libraries(
          ccl-catch
          ccl
          fmt::fmt
          Catch2::Catch2
          Catch2::Catch2WithMain
  )

  if (PRECOMPILED_HEADER)
      target_precompile_headers(
              ccl-catch
              REUSE_FROM
              ccl-lexer
      )

      target_precompile_headers(
              ccl-catch
              PRIVATE
              <ccl/debug/debug_file.hpp>
      )
  endif ()

  add_test(ccl-catch-test ccl-catch)